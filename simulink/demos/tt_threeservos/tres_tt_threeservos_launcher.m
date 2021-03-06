%
%%%%% Sequences of pseudo instructions (task codes)
%
task3_1_descr = { ...
    'fixed(0.002)'; ...
};
task2_1_descr = { ...
    'fixed(0.002)'; ...
};
task1_1_descr = { ...
    'fixed(0.002)'; ...
};

%
%%%%% Description of timing properties of task set
%
%                 % type           %iat     %rdl     %ph     %prio
task_set_descr = { ...
                  'PeriodicTask',  4*0.001    4*0.001    0    0; ...
                  'PeriodicTask',  5*0.001    5*0.001    0    10; ...
                  'PeriodicTask',  6*0.001    6*0.001    0    20; ...
};

% launch the simulink model
uiopen('tres_tt_threeservos.mdl',1);
%sim('tres_tt_threeservos.mdl');
