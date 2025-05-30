#ifndef BUILTINS_H
#define BUILTINS_H

//argc ,argv[] de tach xau
void handle_exit(int argc, char* argv[]);
void handle_help(int argc, char* argv[]);
void handle_date(int argc, char* argv[]);
void handle_time(int argc, char* argv[]);
void handle_ls(int argc, char* argv[]);
void handle_echo_path();
void handle_export(int argc, char* argv[]);

// cd
void handle_cd(const char* path);

#endif