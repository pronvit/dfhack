DFH_PY_FUNC(void, Py_InitializeEx, (int));
DFH_PY_FUNC(int, Py_IsInitialized, (void));
DFH_PY_FUNC(void, Py_Finalize, (void));
DFH_PY_FUNC(void, Py_SetPath, (const wchar_t *));
DFH_PY_FUNC(void, Py_SetProgramName, (wchar_t*));
DFH_PY_FUNC(int, PyRun_SimpleFileExFlags, (void));
DFH_PY_FUNC(int, PyRun_SimpleStringFlags, (const char*, CPython::PyCompilerFlags*));
