# run tests progression
List of tests of the different configurations of the run script.

World | Quiet | Viewer | Control | Command | Status
--- | ---| --- | --- | --- | ---
1on1 | Defaut=False | Defaut=False | Defaut=Keyboard | `Script/run 1on1` |  working
1on1 | Defaut=False | True | Defaut=Keyboard | `Script/run 1on1 -v true` |  working*
1on1 | True | Defaut=True | Defaut=Keyboard | `Script/run 1on1 -q true` | working*
1on1 | True | True | Defaut=Keyboard | `Script/run 1on1 -q true` | working*


*using the generic viewer from the GUI module
