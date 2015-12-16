# About #

PHP security extension to intercept execution of system commands with PHP scripts.
The list of currently intercepted functions:

  * [exec()](http://www.php.net/function.exec)
  * [system()](http://www.php.net/system)
  * [shell\_exec()](http://www.php.net/shell_exec)
  * [passthru()](http://www.php.net/passthru)

Extension is useful for web-hosting to help system administrators to find php-shells and vulnerable php-scripts.

# Installation #

## Requirements ##
  * PHP development files (usually called 'php-dev' in package manager)
  * gcc
  * Subversion

## Build ##

Download extension source from Google SVN repository:
> `svn checkout http://baxtep.googlecode.com/svn/trunk/ baxtep`
Change working directory to the source directory:
> `cd baxtep`
Compile extension:
```
 phpize
 ./configure
 make
```

## Installation ##

After successfull build you'll get baxtep.so in the ./modules/ subdirectory of the source tree. Place it to your PHP extension\_dir and make following changes to your php.ini file:
```
;;;;;;;;;;
; BAXTEP ;
;;;;;;;;;;
; Load extension
extension=baxtep.so

; Absolute path to logfile. File must exists and have write access for php
baxtep.logfile = "/tmp/baxtep_messages"
```
Don't forget to create a file for BAXTEP messages (baxtep.logfile ini setting) and make it writeable for PHP scripts:
```
 touch /tmp/baxtep_messages
 chmod 777 /tmp/baxtep_messages
```
If baxtep.logfile directive is empty and there's no /tmp/baxtep\_messages file you'll get no output from extension.

Sample log string format:
```
2009-03-13 07:59:05 BAXTEP: system CMDLINE: `pwd` FILE: /var/www/html/test.php on line 5 URI: /test.php
```
# TODO & Wishlist #

  1. Better logfile handling (check if we can create a logfile and create it if possible)
  1. Logging via syslog facility
  1. Make log string format customizable via ini file
  1. Implement execution rules (allow/deny execution of commands for some particular scripts based on rules file)