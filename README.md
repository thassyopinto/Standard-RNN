This is a Standard RNN library.

**Activation Function**

<img src="https://render.githubusercontent.com/render/math?math=a^{t%2B1}_{i}=\sigma {(\displaystyle \sum_{j=1}^{n} w_{ij} a^{t}_{j})}">

Possible activation functions:
- Linear: identity, except that it truncates values to lie in [-1, 1]
- Sine: calculates the sine-wave f(x) = sin(x)
- Gaussian: calculates a gaussian f(x) = e^(-x*x), scaled to lie in [-1, 1]
- Sigmoid: calculates the sigmoid f(x)=tanh(x*lambda)

**Testing the Library**

```console
g++ main.cpp -o RNN
```

```console
./RNN [number_of_inputs] [number_of_outputs]
```

**Visualizing the Neural Network**

- Initial RNN

```console
python plotRNN.py std_RNN_ini.csv
```

<p align="center">
  <img src="std_RNN_ini.csv.png" />
</p>

- Evolved RNN

```console
python plotRNN.py std_RNN_mut.csv
```

<p align="center">
  <img src="std_RNN_mut.csv.png" />
</p>
