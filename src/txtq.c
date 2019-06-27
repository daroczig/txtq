#include <R.h>
#include <R_ext/Rdynload.h>
#include <Rinternals.h>
#include <Rversion.h>

SEXP read_text_file(SEXP r_path) {
  const char * path = CHAR(STRING_ELT(r_path, 0));
  FILE *f = fopen(path, "rb");
  if (f == NULL) {
    Rf_error("File '%s' does not exist", path);
  }
  fseek(f, 0L, SEEK_END);
  long size = ftell(f);
  rewind(f);
  char *buf = (char*) malloc(size * sizeof(char));
  if (buf == NULL) {
    Rf_error("Could not read file '%s': error allocating memory", path);
  }
  fread(buf, sizeof(char), size, f);
  fclose(f);
  buf[size] = '\0';
  SEXP out = PROTECT(mkString(buf));
  UNPROTECT(1);
  return out;
}

static const R_CallMethodDef call_methods[] = {
  {"Cread_text_file", (DL_FUNC) &read_text_file, 1},
  {NULL, NULL, 0}
};

void R_init_txtq(DllInfo *dll) {
  R_registerRoutines(dll, NULL, call_methods, NULL, NULL);
#if defined(R_VERSION) && R_VERSION >= R_Version(3, 4, 0)
  R_useDynamicSymbols(dll, FALSE);
  R_forceSymbols(dll, TRUE);
#endif
}
