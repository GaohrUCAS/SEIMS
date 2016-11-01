REM Run SEIMS models under any given scenario IDs
set sceneID=(0,1,2,3,4)
set semsExePath=D:\Compile\SEIMS_OMP\Release
set codePath=C:\z_code\hydro\SEIMS
set modelPath=%codePath%\model_data\dingguang\model_dingguang_30m_longterm
set modelConfig=dingguang_30m_longterm_omp_zhulj_win.ini
set modelConfigPost=post_dianbu_30m_longterm_omp_zhulj_win.ini
set threadNum=6
set layerMethod=0
set IP=127.0.0.1
set port=27017
REM 1. reImport parameters
call python %codePath%\preprocess\import_parameters.py -ini %codePath%\preprocess\%modelConfig%
REM 2. reImport BMP Scenarios
call python %codePath%\preprocess\import_bmp_scenario.py -ini %codePath%\preprocess\%modelConfig%
REM 3. Loop Scenarios
for %%i in %sceneID% do call %semsExePath%\seims_omp %modelPath% %threadNum% %layerMethod% %IP% %port% %%i
REM 4. Plot the outputs of discharge, sediment, and nutrient
call python %codePath%\postprocess\hydroPlot_main.py -ini %codePath%\postprocess\%modelConfigPost%
REM 5. Statistical analysis of the reduction rate of pullutants (COD, TN, and TP)
call python %codePath%\postprocess\run_models_dingguang.py
pause