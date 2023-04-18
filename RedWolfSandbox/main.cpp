#include <iostream>

int main(int argc, char** argv)
{
   try
   {
      std::cout << "Hello RedWolf!\n";
   }
   catch (const std::exception& e)
   {
      std::cout << "Exception: " << e.what() << '\n';
      return 1;
   }
   catch (...)
   {
      std::cout << "Unknown exception\n";
      return 1;
   }

   return 0;
}