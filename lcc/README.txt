A.  COMPILING HYPERMAIL USING LCC-Win32

1.  Install LCC-Win32 in c:\lcc (the default installation directory)

2.  Put the Hypermail directory tree in c:\lcc\projects\hypermail

3.  Bring up wedit, select "Project" and "Create"

4.  Name the project "hypermail"

5.  Make the path to the project's working directory

	c:\lcc\projects\hypermail\lcc

6.  Check to see that the Output Directory is

	c:\lcc\projects\hypermail\lcc\lcc

7.  Click on "Create".

8.  On the "Do you want to use the wizard to generate the application
    skeleton?" dialog, click "No".

9.  On the "Add source files to project" dialog, click "Cancel" 

10. On the "Source files for project" dialog, click "Read file list
    from a text file"

11. Select "hypermail_files.txt"

12. Click on "OK"

13. On the "Compiler settings dialog", click "Next"

14. On the "Linker settings" dialog type "gdbm.lib" in the "Additional
    files to be included in the link" space, then click "Next".

15. On the "Debugger settings" dialog, make any changes you'd like
    to use to begin your debugging session and click "Finish".

16. At this point you should see a lot of text file windows open and
    a dialog labeles "Building makefile for hypermail" with a progress
    bar which moves to the end.

17. Select "Compiler" and "Make".  Observe some fairly meaningless
    error messages and observe the linker return code is "0" (success).


B.  TIPS, BUGS, AND LIMITATIONS ON HYPERMAIL FOR WINDOWS

1.  Hypermail gives a warning message when it attempts to set a locale
    on Windows 98 (and probably 95 and ME).  I believe this is due to a
    limitation in the Windows 9x family of operating systems.  It should
    work OK on NT family operating systems.

2.  LCC version 3.3's standard library gives a blank output for date
    and time when the "%D" format string is specified.  Since "%D" is
    deprecated anyway, I recommend using "%c" instead, which will give
    the same result on U.S. systems.

3.  Don't build hypermail archives in directories that are shortcuts to
    other directories.

4.  The "latest_folder" configuration option is turned off in Windows.

5.  By default, we're building PCRE as an 8-bit library. If you want
    to build it instead as an 16-bit or 32-bit library, change the
    pcre file names in hypermail_files.txt to use the pcre16
    or pcre32 prefix for the following files: pcrenn_maketables.c,
    pcrenn_study.c, pcrenn_get.c, where nn should be 16 or 32.

6.  If you have problems comping the newer pcre library, check
    the NON-AUTOTOOLS-BUILD file for more info and send feedback
    so that we can updat these instructions.

Bob Crispen
revbob@crispen.org

