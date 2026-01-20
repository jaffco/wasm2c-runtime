#include <iostream>
#include "../include/wasm-rt.h"
#include "build/app.h"

// Native implementation of add for comparison
int native_add(int a, int b) {
  return a + b;
}

int main() {
  std::cout << "=== WASM2C Runtime Example ===" << std::endl;
  std::cout << std::endl;

  // Initialize WASM runtime
  std::cout << "Step 1: Initializing WASM runtime..." << std::endl;
  wasm_rt_init();
  std::cout << "  ✓ WASM runtime initialized" << std::endl;
  std::cout << std::endl;

  // Initialize the WASM module
  std::cout << "Step 2: Initializing WASM module (app)..." << std::endl;
  w2c_app app_instance;
  wasm2c_app_instantiate(&app_instance);
  std::cout << "  ✓ WASM module instantiated" << std::endl;
  std::cout << std::endl;

  // Test values
  int a = 15;
  int b = 27;

  std::cout << "Step 3: Running comparison test..." << std::endl;
  std::cout << "  Test inputs: a = " << a << ", b = " << b << std::endl;
  std::cout << std::endl;

  // Call native version
  std::cout << "  → Calling native add(" << a << ", " << b << ")..." << std::endl;
  int native_result = native_add(a, b);
  std::cout << "    Result: " << native_result << std::endl;
  std::cout << std::endl;

  // Call WASM version
  std::cout << "  → Calling WASM add(" << a << ", " << b << ")..." << std::endl;
  int wasm_result = w2c_app_add(&app_instance, a, b);
  std::cout << "    Result: " << wasm_result << std::endl;
  std::cout << std::endl;

  // Compare results
  std::cout << "Step 4: Verifying results..." << std::endl;
  if (native_result == wasm_result) {
    std::cout << "  ✓ SUCCESS: Both implementations returned " << native_result << std::endl;
  } else {
    std::cout << "  ✗ FAILURE: Results differ!" << std::endl;
    std::cout << "    Native: " << native_result << std::endl;
    std::cout << "    WASM:   " << wasm_result << std::endl;
    return 1;
  }
  std::cout << std::endl;

  // Cleanup
  std::cout << "Step 5: Cleaning up..." << std::endl;
  wasm2c_app_free(&app_instance);
  wasm_rt_free();
  std::cout << "  ✓ Resources freed" << std::endl;
  std::cout << std::endl;

  std::cout << "=== Test Complete ===" << std::endl;
  return 0;
}

