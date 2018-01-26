# How to use
## Clone the repository

``` Bash
git clone https://github.com/UFMG-Autobotz/AutoGoal-UFMG
```

## Setup
``` Bash
# run once after cloning the repository
script/setup
```

## Update submodules
``` Bash
# run when you need to get updates from the submodules
# recommended after git pull
script/update
```
If you need to contribute to the submodules, read the [working with submodules](https://github.com/UFMG-Autobotz/AutoGoal-UFMG/wiki/Working-with-submodules) wiki page.


 ## Run demonstrations
 ``` Bash
# run simulation world with 2 robots and play with one against the other
script/run 1on1

 # run simulation world with 2 robots and play against the goalkeeper
script/run penalty

# see more information about how to use this script
script/run --help
 ```
 
