# Blazing HDL

## An HDL that gives you the ability to build hardware blazingly fast!

### Language features

#### Modules
```
mod HalfAdder (
  a <- wire
  b <- wire
  sum -> wire
  carry -> wire
) {
  sum = a ^ b;
  carry = a & b;
}
```

#### Interfaces
```
interface MyFloat {
  sign : bool
  exp : uint
  mant : uint
}

interface UnaryOpIO {
  in <- MyFloat
  out -> MyFloat
}

mod MyDuplicationUnit(io : UnaryOpIO) {
  wire MyFloat result;
  reg MyFloat pipe;
  result.significand = io.in.significand;
  result.sign = io.in.sign;
  result.exponent = io.in.exponent + 1;
  pipe = result;
  io.out = pipe;
}
```
