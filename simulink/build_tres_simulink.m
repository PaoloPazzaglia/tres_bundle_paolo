% DO NOT MODIFY BELOW!
% ====================
LIB_SFX  = '/';
if ispc,
    LIB_SFX  = '/Release';
end
TRES_BASE_INC    = [pwd,'/../base/include/ -I', ...
                    pwd,'/../base/src'];
TRES_RTSIM_INC   = [pwd,'/../adapters/rtsim/include/'];
RTSIM_INC        = [pwd,'/../3rdparty/rtsim/metasim/src/ -I', ...
                    pwd,'/../3rdparty/rtsim/rtlib/src/'];
TRES_BASE_LIB    = [pwd,'/../build/base/src/'];
TRES_RTSIM_LIB   = [pwd,'/../build/adapters/rtsim/src/'];
METASIM_LIB      = [pwd,'/../build/3rdparty/rtsim/metasim/src/'];
RTLIB_LIB        = [pwd,'/../build/3rdparty/rtsim/rtlib/src/'];
TRES_DBG         = ''; %-g -D_DEBUG';
%MEX_CFLAGS       = 'COMPFLAGS=''/c /GR /W3 /EHs /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /D_SECURE_SCL=0 /DMATLAB_MEX_FILE /nologo /MDd''';
MEX_CFLAGS       = 'CXXFLAGS=''$CXXFLAGS -Wall -O2 -std=c++0x''';
MEX_INC          = sprintf('-I%s -I%s -I%s', RTSIM_INC, TRES_BASE_INC, TRES_RTSIM_INC);
MEX_LIB          = sprintf('-L%s -L%s -L%s -L%s -ltres_base -ltres_rtsim -lmetasim -lrtlib', ...
                            [TRES_BASE_LIB,LIB_SFX], [TRES_RTSIM_LIB,LIB_SFX], [RTLIB_LIB,LIB_SFX], [METASIM_LIB,LIB_SFX]);
MEX_OUT      = '-outdir libs';
MEX_IN_CLL   = {MEX_OUT, MEX_CFLAGS, TRES_DBG, MEX_INC, MEX_LIB};
MEX_IN_CMD   = [sprintf('%s ',MEX_IN_CLL{1:end-1}), MEX_IN_CLL{end}];
MDL_SRC      = {'src/common/tres_enabler_df.cpp', ...
                'src/node/tres_task.cpp', ...
                'src/node/tres_kernel.cpp', ...
                'src/node/tres_task_df.cpp',...
                'src/node/tres_task_df_AVR.cpp'};
                
if ~exist('libs','dir'),
    mkdir('libs');
end

disp ('Building T-Res mex files...');
cellfun(@(sfun) eval(sprintf('mex -v %s %s', MEX_IN_CMD, sfun)), MDL_SRC, 'UniformOutput', true);
disp ('Building T-Res mex files... DONE!');
clear LD_PATH RTSIM_INC TRES_BASE_INC TRES_RTSIM_INC TRES_BASE_LIB TRES_RTSIM_LIB RTLIB_LIB METASIM_LIB TRES_DBG MEX_CFLAGS MEX_INC MEX_LIB MDL_SRC MEX_IN_CLL MEX_IN_CMD MEX_OUT LIB_SFX
