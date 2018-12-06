#include <iostream>
#include <memory>

int main() {
  std::shared_ptr<int> a;
  // Because the shared_ptr class reload the bool()
  // explicit operator bool() const noexcept;
  if (a) {
    std::cout << "true" << std::endl;
  } else {
    std::cout << "false" << std::endl;
  }

  a = std::make_shared<int>(5);
  if (a) {
    std::cout << "true" << std::endl;
  } else {
    std::cout << "false" << std::endl;
  }

  return 0;
}
