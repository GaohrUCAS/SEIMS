REM Run dianbu, dianbu2, and dianbu3 on scenario 2 (demo extended scenario) and 3 (base scenario), respectively
REM And get the statistics.
REM Import Parameters
call python C:\z_code\hydro\SEIMS\preprocess\import_parameters.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu_30m_longterm_omp_zhulj_win.ini
call python C:\z_code\hydro\SEIMS\preprocess\import_parameters.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu2_30m_longterm_omp_zhulj_win.ini
call python C:\z_code\hydro\SEIMS\preprocess\import_parameters.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu3_30m_longterm_omp_zhulj_win.ini
REM Import Scenarios
call python C:\z_code\hydro\SEIMS\preprocess\import_bmp_scenario.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu_30m_longterm_omp_zhulj_win.ini
call python C:\z_code\hydro\SEIMS\preprocess\import_bmp_scenario.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu2_30m_longterm_omp_zhulj_win.ini
call python C:\z_code\hydro\SEIMS\preprocess\import_bmp_scenario.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu3_30m_longterm_omp_zhulj_win.ini
REM RUN MODELS
call D:\Compile\SEIMS_OMP\Release\seims_omp C:\z_code\Hydro\SEIMS\model_data\dianbu\model_dianbu_30m_longterm 6 0 127.0.0.1 27017 2
call D:\Compile\SEIMS_OMP\Release\seims_omp C:\z_code\Hydro\SEIMS\model_data\dianbu\model_dianbu_30m_longterm 6 0 127.0.0.1 27017 3
call D:\Compile\SEIMS_OMP\Release\seims_omp C:\z_code\Hydro\SEIMS\model_data\dianbu\model_dianbu2_30m_longterm 6 0 127.0.0.1 27017 2
call D:\Compile\SEIMS_OMP\Release\seims_omp C:\z_code\Hydro\SEIMS\model_data\dianbu\model_dianbu2_30m_longterm 6 0 127.0.0.1 27017 3
call D:\Compile\SEIMS_OMP\Release\seims_omp C:\z_code\Hydro\SEIMS\model_data\dianbu\model_dianbu3_30m_longterm 6 0 127.0.0.1 27017 2
call D:\Compile\SEIMS_OMP\Release\seims_omp C:\z_code\Hydro\SEIMS\model_data\dianbu\model_dianbu3_30m_longterm 6 0 127.0.0.1 27017 3
call python C:\z_code\Hydro\SEIMS\postprocess\run_models.py
pause