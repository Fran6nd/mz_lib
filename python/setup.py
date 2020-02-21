from distutils.core import setup, Extension

def main():
    setup(name="mz_lib",
          version="1.0.0",
          description="Python interface for the mz_lib C library function",
          author="François NOUAILLE DEGORCE",
          author_email="nouaille.degorce.francois@gmail.com",
          ext_modules=[Extension("mz_lib", ["mz_lib_py.c"])])

if __name__ == "__main__":
    main()

