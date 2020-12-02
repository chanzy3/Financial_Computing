template<class T> void cfl::test::print(T start, T end) 
{
  for (T i=start; i<end; i++) {
    std::cout << "[" << (i-start) << "]" << " = " << *i << std::endl;
  }
  cfl::test::pause();
  std::cout << std::endl;
}
