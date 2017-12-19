# run tests progression
List of tests of the different configurations of the run script.

##### Legend:
* :white_check_mark: working
* :warning: not tested
* :x: not working

World | Quiet | Viewer | Control | Command | Status
--- | ---| --- | --- | --- | ---
<!-- false, false, keyboard -->
1on1 | Defaut=False | Defaut=False | Defaut=Keyboard | `Script/run 1on1` |  :white_check_mark:
1on1 | Defaut=False | Defaut=False | Keyboard | `Script/run 1on1 -c keyboard` |  :warning:
1on1 | Defaut=False | False | Defaut=Keyboard | `Script/run 1on1 -v false` |  :warning:
1on1 | Defaut=False | False | Keyboard | `Script/run 1on1 -v false -c keyboard` |  :warning:
1on1 | False | Defaut=False | Defaut=Keyboard | `Script/run 1on1 -q false` |  :white_check_mark:
1on1 | False | False | Defaut=Keyboard | `Script/run 1on1 -q false -v false` |  :warning:
1on1 | False | Default=False | Keyboard | `Script/run 1on1 -q false -c keyboard` |  :warning:
1on1 | False | False | Keyboard | `Script/run 1on1 -q false -v false -c keyboard` |  :white_check_mark:
<!-- teste -->
1on1 | Defaut=False | True | Defaut=Keyboard | `Script/run 1on1 -v true` |  :white_check_mark:*
1on1 | True | Defaut=True | Defaut=Keyboard | `Script/run 1on1 -q true` | :white_check_mark:*
1on1 | True | True | Defaut=Keyboard | `Script/run 1on1 -q true -v true` | :white_check_mark:*


*using the generic viewer from the GUI module
