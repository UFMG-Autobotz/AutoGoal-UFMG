# run tests progression
List of tests of the different configurations of the run script.

World | Quiet | Viewer | Control | Command | Status
--- | ---| --- | --- | --- | ---
1on1 | Defaut=False | Defaut=False | Defaut=Keyboard | `Script/run 1on1` |  working
1on1 | Defaut=False | Defaut=False | Keyboard | `Script/run 1on1 -c keyboard` |  not tested
1on1 | Defaut=False | False | Defaut=Keyboard | `Script/run 1on1 -v false` |  not tested
1on1 | Defaut=False | False | Keyboard | `Script/run 1on1 -v false -c keyboard` |  not tested
1on1 | False | Defaut=False | Defaut=Keyboard | `Script/run 1on1 -q false` |  working
1on1 | False | False | Defaut=Keyboard | `Script/run 1on1 -q false -v false` |  not tested
1on1 | False | Default=False | Keyboard | `Script/run 1on1 -q false -c keyboard` |  not tested
1on1 | False | False | Keyboard | `Script/run 1on1 -q false -v false -c keyboard` |  working
<!-- teste -->
1on1 | Defaut=False | True | Defaut=Keyboard | `Script/run 1on1 -v true` |  working*
1on1 | True | Defaut=True | Defaut=Keyboard | `Script/run 1on1 -q true` | working*
1on1 | True | True | Defaut=Keyboard | `Script/run 1on1 -q true -v true` | working*


*using the generic viewer from the GUI module
