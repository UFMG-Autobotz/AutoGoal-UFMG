# run tests progression
List of tests of the different configurations of the run script.

##### Legend:
* :white_check_mark: working
* :warning: not tested
* :x: not working

###### 1on1 | not quiet | no viewer | keyboard
World | Quiet | Viewer | Control | Command | Status
--- | ---| --- | --- | --- | ---
1on1 | Defaut | Defaut | Defaut | `Script/run 1on1` |  :white_check_mark:
1on1 | Defaut | Defaut | Keyboard | `Script/run 1on1 -c keyboard` |  :warning:
1on1 | Defaut | False | Defaut | `Script/run 1on1 -v false` |  :warning:
1on1 | Defaut | False | Keyboard | `Script/run 1on1 -v false -c keyboard` |  :warning:
1on1 | False | Defaut | Defaut | `Script/run 1on1 -q false` |  :white_check_mark:
1on1 | False | False | Defaut | `Script/run 1on1 -q false -v false` |  :warning:
1on1 | False | Default | Keyboard | `Script/run 1on1 -q false -c keyboard` |  :warning:
1on1 | False | False | Keyboard | `Script/run 1on1 -q false -v false -c keyboard` |  :white_check_mark:

###### 1on1 | not quiet | viewer | keyboard
World | Quiet | Viewer | Control | Command | Status
--- | ---| --- | --- | --- | ---
1on1 | Defaut | True | Defaut | `Script/run 1on1 -v true` |  :white_check_mark:<sup>1</sup>
1on1 | False | True | Defaut | `Script/run 1on1 -q false -v true` |  :warning:
1on1 | Defaut | True | Keyboard | `Script/run 1on1 -v true -c keyboard` |  :warning:
1on1 | False | True | Keyboard | `Script/run 1on1 -q false -v true -c keyboard` |  :warning:

###### 1on1 | quiet | viewer | keyboard
World | Quiet | Viewer | Control | Command | Status
--- | ---| --- | --- | --- | ---
1on1 | True | Defaut | Defaut | `Script/run 1on1 -q true` | :white_check_mark:<sup>1</sup>
1on1 | True | True | Defaut | `Script/run 1on1 -q true -v true` | :white_check_mark:<sup>1</sup>
1on1 | True | Defaut | Keyboard | `Script/run 1on1 -q true -c keyboard` | :warning:
1on1 | True | True | Keyboard | `Script/run 1on1 -q true -v true -c keyboard` | :warning:

<sup>1</sup>using the generic viewer from the GUI module
