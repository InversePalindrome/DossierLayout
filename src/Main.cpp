/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Main.cpp
InversePalindrome.com
*/


#include "Application.hpp"


int main(int argc, char *argv[])
{
   QCoreApplication::addLibraryPath(".");

   Application app(argc, argv);

   return app.run();
}
