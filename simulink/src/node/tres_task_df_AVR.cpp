/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2014,2015, ReTiS Lab., Scuola Superiore Sant'Anna.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the ReTiS Lab. nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/**
 * \file tres_task.cpp
 */

// TODO: license, doxygen
#define S_FUNCTION_NAME tres_task_df_AVR
#define S_FUNCTION_LEVEL 2

#include <iostream>
#include <string>
#include <vector>
#include <tres/Task.hpp>
#include "simstruc.h"

#include "reginstr.cpp"
#include "regvar.cpp"

#define INSTRSET_DESCR_VARNAME 0
#define MODES_VARNAME 1

static const mxArray * getTaskInstructionSet(SimStruct *S)
{
    char *bufIsd;       // INSTRSET_DESCR_VARNAME
    int bufIsdLen;
    const mxArray *mxIsd;

    // Get the instruction-set description (Isd)
    // (block's callbacks guarantee it's a MATLAB cell array)
    bufIsdLen = mxGetN( ssGetSFcnParam(S,INSTRSET_DESCR_VARNAME) )+1;
    bufIsd = new char[bufIsdLen];
    mxGetString(ssGetSFcnParam(S,INSTRSET_DESCR_VARNAME), bufIsd, bufIsdLen);
    mxIsd = mexGetVariablePtr("base", bufIsd);
    delete bufIsd;

    return mxIsd;
}

/* Function: mdlInitializeSizes ===========================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    int num_segments;
    
    // Get the number of modes
    int num_of_modes = static_cast<int>(mxGetScalar(ssGetSFcnParam(S, MODES_VARNAME)));
   
    // Get the number of segments
    num_segments = mxGetNumberOfElements(getTaskInstructionSet(S))/num_of_modes;
    mexPrintf("number of segments: %d\n",num_segments);
    
    ssSetNumSFcnParams(S, 2);  /* Number of expected parameters */
    // task_AVR_instr and number_of_modes

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

     // Set the input port
    if (!ssSetNumInputPorts(S, 1)) return; // input: Mode (it's a positive integer)
    ssSetInputPortWidth(S, 0, 1);
    // Set the input ports as Direct Feed Through
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    if(!ssSetInputPortDataType(S, 0, DYNAMICALLY_TYPED)) return;
    
    // Set the output ports
    if (!ssSetNumOutputPorts(S, 2)) return;
    ssSetOutputPortWidth(S, 0, 2*num_segments);
    ssSetOutputPortWidth(S, 1, 1);
    
    ssSetNumSampleTimes(S, 1);
            
    ssSetNumPWork(S, num_of_modes+1); // tres::Task (set in mdlInitializeConditions())
    ssSetNumRWork(S, 5); // store the value to be issued on the data-flow
    
    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);
}

/* Function: mdlInitializeSampleTimes =====================================
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    // Sample times
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, FIXED_IN_MINOR_STEP_OFFSET);
   
}

#define MDL_INITIALIZE_CONDITIONS
/* Function: mdlInitializeConditions ======================================
 * Abstract:
 *    Initialize discrete state to zero.
 */
static void mdlInitializeConditions(SimStruct *S)
{
    const mxArray *mxIsd;
    int num_segments;
    std::vector<std::string> instr_list;
    int mode;
    int dyn_index;

    // Get the number of modes
    int num_of_modes = static_cast<int>(mxGetScalar(ssGetSFcnParam(S, MODES_VARNAME)));
     
    // Extract mode from input
    InputRealPtrsType mode_pointer = ssGetInputPortRealSignalPtrs(S,0);
    mode = static_cast<int>(*mode_pointer[0]);
    
    // ERROR CHECK
    // Resolve crytical values for mode
    if (mode<=0) mode = 1;
    // Resolve maximum mode number exceeded
    if (mode>num_of_modes){
        mode = num_of_modes;
        ssWarning(S, "Warning: Mode input exceeded its limits, variable set to its maximum value");
    }
    mexPrintf("active mode is number %d\n",mode);
    
    // Save num__of_modes to RWork vector for later use
    ssGetRWork(S)[0] = num_of_modes;
    
    // Get the number of segments
    mxIsd = getTaskInstructionSet(S);
    num_segments = mxGetNumberOfElements(mxIsd)/num_of_modes;
    
    // Save num of segments to RWork vector for later use
    ssGetRWork(S)[1] = num_segments;
    
    // Save the current mode (will be used as previous mode in mdlOutputs)
    ssGetRWork(S)[2] = mode;
    
    // Save the active index (initialized as 0)
    ssGetRWork(S)[3] = 0;
        
    // Initialize number of skipped instructions of the type 'fixed(0)'
    ssGetRWork(S)[4] = 0;
    
    // Initialize vector of tasks, one for each mode's instruction
    std::vector<tres::Task *> tasklist; 
    tres::Task *task;
    
    // Construct the sequence of pseudo instructions
    // Note that the consistency of the task-code description (cell array)
    // has already been checked in get_sfun_param()
    for (int mode_index = 0; mode_index < num_of_modes; mode_index++){
        
        for (int index = 0; index < num_segments; index++)
        {
            dyn_index = mode_index+index*num_of_modes; //dynamic index extracting row index from column formattation
            instr_list.push_back( std::string(mxArrayToString(mxGetCell(mxIsd, dyn_index))) );
        }
        // Create a new tres::Task instance from the task-code description vector
        task = new tres::Task(instr_list);
        tasklist.push_back(task);
                
        // Save the new C++ object for later usage (mdlOutputs())
        ssGetPWork(S)[mode_index] = tasklist.at(mode_index);
        
        // Clear the instr_list
        instr_list.clear(); 
    }

    // Initialize the (1st) standard output port (enable signal for segments)
    real_T *y = ssGetOutputPortRealSignal(S,0);
    for (int i = 0; i < num_segments; i++){
        y[i] = 0.0;
    }
    
    task = static_cast<tres::Task *>(ssGetPWork(S)[mode-1]);
    
    // Initial the 2nd (real) Output
    y = ssGetOutputPortRealSignal(S,1);
#ifdef TRES_DEBUG_1
        mexPrintf("%s: task->ni_duration is %.3f\n", __FILE__, task->getSegmentDuration());
#endif
    y[0] = task->getSegmentDuration();
}


/* Function: mdlOutputs ===================================================
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{    
    // Extract num_of_modes from RWork vector
    int num_of_modes = ssGetRWork(S)[0];

    // Extract number of segments from RWork vector
    int num_segments = ssGetRWork(S)[1];
    
    // Extract previous mode from RWork vector
    int prev_mode = ssGetRWork(S)[2];
    
    // Extract previous subsys index from RWork vector
    int prev_subsys_idx = ssGetRWork(S)[3];    
    
    // Extract active mode from input
    InputRealPtrsType mode_pointer = ssGetInputPortRealSignalPtrs(S,0);
    int mode = static_cast<int>(*mode_pointer[0]);
    
    // ERROR CHECK
    // Resolve crytical values for mode
    if (mode<=0) mode = 1;
    // Resolve maximum mode number exceeded
    if (mode>num_of_modes){
        mode = num_of_modes;
        ssWarning(S, "Warning: Mode input exceeded its limits, variable set to its maximum value");
    }
    mexPrintf("active mode is number %d\n",mode);
    
    // Get the C++ object back from the pointers vector
    tres::Task *task = static_cast<tres::Task *>(ssGetPWork(S)[mode-1]);
    
    // Process the current segment
    int subsys_idx;
    do {
        subsys_idx = task->processSegment();
    // This assure to run next instruction also when mode has changed!
    } while ((prev_mode != mode) && (((prev_subsys_idx+1)%(num_segments+1)) != subsys_idx));
    
    // Issue the enable signal (subsys_act_idx) 
    real_T  *y = ssGetOutputPortRealSignal(S,0);
    mexPrintf( "%s, %s, At time %.16f\n", ssGetPath(S), __FUNCTION__, ssGetT(S) );
      
    // Activate instruction signal
    if (subsys_idx == 0)
        y[0] = 1.0;
    else
    {      
        y[2*subsys_idx-1] = 1.0;
        if (subsys_idx < task->getNumberOfSegments())
            y[2*subsys_idx] = 1.0;
    }
     
    // Check if there are skipped instruction ('fixed(0)')to be activated
    int skipped_instr = ssGetRWork(S)[4];
    
    if (skipped_instr !=0){
        for (int skip_idx = 1; skip_idx <= skipped_instr; skip_idx++){
            // Release signal for skipped istructions (activ+latch)
            y[2*(subsys_idx - skip_idx)-1] = 1.0;
            y[2*(subsys_idx - skip_idx)] = 1.0;
        }
        // Reset skipped_instr counter
        skipped_instr = 0;
    }
   
    // Check if next instruction is 'fixed(0)': if yes, switch to next 
    // instruction and store how many instructions to skip have been found
    while (task->getSegmentDuration()==0){
                subsys_idx = task->processSegment();
                skipped_instr++;
    }    
    
    // Write (duration of next_instr)
    y = ssGetOutputPortRealSignal(S,1);
    y[0] = task->getSegmentDuration();


    // Update current mode
    ssGetRWork(S)[2] = mode;
    // Update current active segment
    ssGetRWork(S)[3] = subsys_idx;
    // Update number of skipped instructions
    ssGetRWork(S)[4] = skipped_instr;
    
    mexPrintf("%s: activated segment #%d task->ni_duration is %.3f\n", __FILE__,
        subsys_idx, task->getSegmentDuration());
        
}

/* Function: mdlTerminate =================================================
 *    No termination needed, but we are required to have this routine.
 */
static void mdlTerminate(SimStruct *S)
{    
    // Extract num_of_modes from RWork vector
    int num_of_modes =  ssGetRWork(S)[0];
    
    // Retrieve and destroy all C++ objects
    for (int i = 0; i < num_of_modes; i++){
        tres::Task *task = static_cast<tres::Task *>(ssGetPWork(S)[i]);
        delete task;
    }
}

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif

