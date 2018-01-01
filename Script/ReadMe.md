# Scripts to rule them all

These scripts are inspired by the [normalized script pattern that GitHub uses in its projects](https://githubengineering.com/scripts-to-rule-them-all/).
Here's a description of what each script used in this project does.


## Script/setup
> sets up a project to be used for the first time

Use it once after cloning the project to:
* create useful aliases;
* download submodules;
* prepare submodules to allow your contribution.


## Script/update
> updates a project to run at its current version

Use it after `git pull` to:
* update submodules;
* recompile all plugins of the Autobotz-Simulator submodule.

## Script/run
> runs demonstration simulations with a single command

#### Usage
`Script/run world [options]`

#### Worlds implemented
* penalty
* 1on1

#### Options
* -q: **quiet** - boolean, default is false
* -v: **viewer** - boolean, default is false when not quiet and true when quiet
* -c: **control** - keyboard or none, default is keyboard

#### Example
```
# run world 1on1 on quiet mode (with viewer enabled and keyboard control by default)
Script/run 1on1 -q true
```
