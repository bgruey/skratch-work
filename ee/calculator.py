

from ee.linear_electronics.resistors import parallel_resistor_tolerance_calculator

for i in range(20):
    r, dr = parallel_resistor_tolerance_calculator(5000, 500, i+1)
    print(f"{i}: {r:.1f} +/- {dr:.2e} at {100.0 * dr / r:.1f} percent uncertainty")