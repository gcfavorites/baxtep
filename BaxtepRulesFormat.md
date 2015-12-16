# Format #1 #

**allow|deny** _regular\_expression_ `[`_options_`]`

I.e:
```
allow ^myapp /[\w/]+$ [L]
deny ^nohup []
deny &$ [FL]
```

Possible options:
  * i - case insensitive match
  * L - log this match
  * l - never log this match
  * F - throw fatal error (valid only for **deny** action)
  * f - never throw fatal error

There is 2 options in php.ini to change rules' behaviour:
  * baxtep.force\_log - always log matched rule (except with **l** option)
  * baxtep.fatal\_on\_deny - always throw fatal error on **deny** action (except with **f** option)

**...TO BE CONTINUED**