% DO NOT MODIFY BELOW!
% ====================
tres_loader_path = mfilename('fullpath');
bslash_idx = strfind(tres_loader_path, '\');
tres_simulink_dir = tres_loader_path(1:bslash_idx(end)-1);
setenv('TRES_SIMULINK_DIR', tres_simulink_dir);

addpath(genpath([getenv('TRES_SIMULINK_DIR'), '/blockset']));
addpath([getenv('TRES_SIMULINK_DIR'), '/libs']);

clear tres_loader_path bslash_idx tres_simulink_dir