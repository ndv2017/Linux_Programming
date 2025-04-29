**BT1**. Write a program that opens any file using the `O_APPEND` flag, then performs a *seek* to the beginning of the file and writes some data to that file. Where will the data appear in the file and why?

*Answer*:

The data will be written at the **end of the file**, **not at the beginning**, despite calling `lseek()` to move the pointer to the beginning.

The reason is because the file was opened with the `O_APPEND` flag. When using `O_APPEND`, each write operation (`write()`) will **automatically write to the end of the file**, regardless of where the file pointer is positioned. Therefore, the `lseek()` command has no effect on the write position in this case.
