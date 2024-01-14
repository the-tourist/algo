template <typename T>
struct PrimitiveVarMod { static T value; };
template <typename T>
T PrimitiveVarMod<T>::value;

template <typename T, class F>
T GetPrimitiveRoot(const T& modulo, const F& factorize) {
  if (modulo <= 0) {
    return -1;
  }
  if (modulo == 1 || modulo == 2 || modulo == 4) {
    return modulo - 1;
  }
  vector<pair<T, int>> modulo_factors = factorize(modulo);
  if (modulo_factors[0].first == 2 && (modulo_factors[0].second != 1 || modulo_factors.size() != 2)) {
    return -1;
  }
  if (modulo_factors[0].first != 2 && modulo_factors.size() != 1) {
    return -1;
  }
  set<T> phi_factors;
  T phi = modulo;
  for (auto& d : modulo_factors) {
    phi = phi / d.first * (d.first - 1);
    if (d.second > 1) {
      phi_factors.insert(d.first);
    }
    for (auto& e : factorize(d.first - 1)) {
      phi_factors.insert(e.first);
    }
  }
  PrimitiveVarMod<T>::value = modulo;
  Modular<PrimitiveVarMod<T>> gen = 2;
  while (gen != 0) {
    if (power(gen, phi) != 1) {
      continue;
    }
    bool ok = true;
    for (auto& p : phi_factors) {
       if (power(gen, phi / p) == 1) {
         ok = false;
         break;
       }
     }
    if (ok) {
      return gen();
    }
    gen++;
  }
  assert(false);
  return -1;
}

template <typename T>
T GetPrimitiveRoot(const T& modulo) {
  return GetPrimitiveRoot(modulo, factorizer::Factorize<T>);
}