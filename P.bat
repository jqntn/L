copy x64\Release\L.exe
copy x64\Release\D.exe
tar -acf L.zip L.exe D.exe
del L.exe
del D.exe