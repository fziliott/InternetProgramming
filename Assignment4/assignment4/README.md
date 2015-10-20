# Environment setup for Debian

The following comands have been tested on Debian Linux Jessie (current stable).
Commands preceded with the `#` prompt must be run as root. Commands preceded with the `$` prompt must be run as user. To switch to root user, use one of the following commands - depending on your setup:

```console
$ sudo su -
```

or

```
$ su -
```

## Packages installation and setup

### Remove and reinstall Apache
If you have already setup Apache, it may be a good idea to make a fresh installation so that the following steps apply directly on your setup. **This is up to you to decide.** The following commands will remove Apache and its related packages along with their configuration:

```console
# apt-get remove --purge apache2 libapache2-mod-php5
# apt-get autoremove --purge
```

Then reinstall Apache with PHP support:

```console
# apt-get install apache2 libapache2-mod-php5
```

### Configure Apache
In oreder to work from your user directory, you need to enable the `userdir` Apache module. You also need to enable the `cgi` module. Use the following commands:

```console
# a2enmod userdir
# a2enmod cgi
```

### Enable PHP for user directories
PHP is by default disabled for users. You need to edit `/etc/apache2/mods-enabled/php5.conf` in order to enable it. Use a text editor to open the file and comment-out the `<IfModule mod_userdir.c>...</IfModule>` block at the bottom of the file.

### Enable CGI for user directorues.
CGI also has to be enabled for users. Edit file `/etc/apache2/mods-enabled/userdir.conf` and add the following configuration block below the existing `<Directory>...</Directory>` block:

```apache
<Directory /home/*/public_html/cgi-bin>
 	Options ExecCGI
	SetHandler cgi-script
</Directory>
```

### Restart Apache
Use the following command to restart your Apache server and apply the changes we made above.

``` console
# service apache2 restart
```

### Debugging
When things don't work ass expected, you may get useful information from the Apache log files.

**Error log:**
```console
# tail -f /var/log/apache2/error.log
```

**Access log:**
```console
# tail -f  /var/log/apache2/access.log
```

## Setup user web directory

### Test setup
User web directories are served from `~/public_html` and served e.g. from http://127.0.0.1/~user, if your username is `user`. To test that everything working, use the following commands and visit your user page. It should read `Hello world!`.

```console
$ [ -d ~/public_html ] && mv -f ~/public_html ~/public_html.old
$ mkdir ~/public_html
$ echo '<?php echo("Hello world!"); ?>' > ~/public_html/index.php
```

If you see the message, you can remove the test `public_html`.
```console
$ rm -rf ~/public_html
```

If the message isn't displayed, check your Apache error logs for hints on what went wrong.

### Working setup
To setup your user working environment, you first need to clone the `intprog-samples` repository.

```
$ git clone https://github.com/m000/intprog-samples
```

Then link the `web` directory to your `public_html`.

```
$ ln -s intprog-samples/assignment4/web ~/public_html
```

Finally, run `make` from the assignment directory to generate configuration and setup [Smarty template engine](http://www.smarty.net/).

```
$ cd intprog-samples/assignment4
$ make
```

You are all set!
