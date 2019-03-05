# Compiler
CC=cl.exe
# Linker
LINK=link.exe
# Compiler flags
CFLAGS=/c

# Sources directory
SRC_DIR=sourcecode\src
# Objects directory
OBJ_DIR=sourcecode\obj
# Exectuable directory
BIN_DIR=executable
# Include directory
INC_DIR=sourcecode\include

# Executable
EXE=game.exe

# All
all : clean create_dirs make_exe

# All + run executable immediately
run : clean create_dirs make_exe run_exe

# Link the object files into a binary
make_exe : $(EXE)

# Open created executable
run_exe :
	@if exist $(BIN_DIR)\$(EXE) $(BIN_DIR)\$(EXE)

# Compile each .c file to its respective .obj file
{$(SRC_DIR)}.c{$(OBJ_DIR)}.obj :
	$(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /I$(INC_DIR)\ $<

# Link the files into an executable
$(EXE): $(OBJ_DIR)\*.obj
	$(LINK) /out:$(BIN_DIR)\$(EXE) $(OBJ_DIR)\*.obj

# Create output directories
create_dirs :
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

# Clean output directories
clean :
	@if exist $(BIN_DIR)\$(EXE) del $(BIN_DIR)\$(EXE)
	@if exist $(OBJ_DIR)\*.obj del $(OBJ_DIR)\*.obj
	@if exist *.replay del *.replay
	@if exist $(BIN_DIR)\*.replay del $(BIN_DIR)\*.replay