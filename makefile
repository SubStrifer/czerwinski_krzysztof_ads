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
INC=sourcecode\include

# Executable
EXE=game.exe

all : clean create_dirs make_exe

# Link the object files into a binary
make_exe : $(EXE)

# Compile each .c file to its respective .obj file
{$(SRC_DIR)}.c{$(OBJ_DIR)}.obj :
	$(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /I$(INC)\ $<

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
	@if exist $(OBJ_DIR) rmdir /S /Q $(OBJ_DIR)
