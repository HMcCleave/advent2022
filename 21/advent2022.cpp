#include "all_helpers.hpp"

using namespace std;

ifstream _cin("input.txt");

int64_t gcd_(int64_t a, int64_t b) {
  if (b) {
    return gcd_(b, a % b);
  }
  return a;
}

int64_t gcd(int64_t a, int64_t b) {
  return gcd_(max(abs(a), abs(b)), min(abs(a), abs(b)));
}


struct Op {
  string a, b;
  int64_t opt;
};

map<string, variant<int64_t, Op>> input;


const int64_t kMul = (int64_t)'*';
const int64_t kAdd = (int64_t)'+';
const int64_t kSub = (int64_t)'-';
const int64_t kDiv = (int64_t)'/';


ostream& operator<<(ostream& out, variant<int64_t, Op> op) {
  struct {
    void operator()(int64_t x) { cout << x << endl; }
    void operator()(Op o) {
      cout << o.a << ' ' << (char)o.opt << ' ' << o.b << endl;
    }
  } visitor;
  
  std::visit(visitor, op);

  return out;
}

void read() {
  string s;
  while (getline(_cin, s)) {
    stringstream ss(s);

    string tag;
    int64_t value;
    if (ss >> tag >> value) {
      tag = tag.substr(0, tag.length() - 1);
      input[tag] = {value};
    } else {
      string a, b;
      char op;
      ss = stringstream(s);
      ss >> tag >> a >> op >> b;
      tag = tag.substr(0, tag.length() - 1);

      input[tag] = Op{a, b, (int64_t)op};
    }

    //cout << tag << " = "  << input[tag];
  }
}

optional<int64_t> eval(string tag) {
  struct {
    optional<int64_t> operator()(int64_t x) { return x; }
    optional<int64_t> operator()(Op o) {
      auto _A = eval(o.a);
      auto _B = eval(o.b);

      if (!(_A.has_value() || _B.has_value()))
        cout << "oh god no" << endl;

      if (!_A.has_value())
        return nullopt;
      if (!_B.has_value())
        return nullopt;
      auto A = _A.value();
      auto B = _B.value();
      switch (o.opt) { case kMul:
          return A * B;
        case kAdd:
          return A + B;
        case kSub:
          return A - B;
        case kDiv:
          return A / B;
        default:
          cout << "~!!!!!!" << endl;
          return nullopt;
      }
    }
  } visitor;

  if (input.find(tag) == input.end())
    return nullopt;

  return visit(visitor, input[tag]);
}

pair<int64_t, int64_t> eval2(string tag, pair<int64_t, int64_t> equals) {
  if (tag == "humn")
    return equals;

  auto [rNum, rDiv] = equals;
  auto g = gcd(rNum, rDiv);
  rNum /= g;
  rDiv /= g;

  auto op = get<Op>(input[tag]);
  auto _A = eval(op.a);
  auto _B = eval(op.b);
  if (_A.has_value()) {
    switch (op.opt) {
    case kAdd:
        return eval2(op.b, {rNum - (rDiv * _A.value()), rDiv});
    case kSub:
      return eval2(op.b, {(rDiv * _A.value()) - rNum, rDiv});
    case kMul:
      return eval2(op.b, {rNum, rDiv * _A.value()});
    case kDiv:
      return eval2(op.b, {_A.value() * rDiv, rNum});
    }
  }
  int64_t v = _B.value();
  switch (op.opt) {
    case kAdd:
      return eval2(op.a, {rNum - (v * rDiv), rDiv});
    case kSub:
      return eval2(op.a, {(v * rDiv) + rNum, rDiv});
    case kMul:
      return eval2(op.a, {rNum, rDiv * v});
    case kDiv:
      return eval2(op.a, {rNum * v, rDiv});
  }

  cout << "?";
  return {-1, -1};
}

void first() {
  read();
  cout << eval("root").value() << endl;
}

/*nope, binaryish search didnt work*/
int64_t guestimate(string a, string b, int64_t low, int64_t high) {
  auto mid = (low + high) / 2;
  int64_t low_diff, mid_diff, high_diff;
  input["humn"] = low;
  low_diff = abs(eval(a).value() - eval(b).value());
  if (low_diff == 0)
    return low;
  input["humn"] = mid;
  mid_diff = abs(eval(a).value() - eval(b).value());
  if (mid_diff == 0)
    return mid;
  input["humn"] = high;
  high_diff = abs(eval(a).value() - eval(b).value());
  if (high_diff == 0)
    return high;
  if (low_diff < high_diff)
    return guestimate(a, b, low, mid);
  return guestimate(a, b, mid, high);
}


void second() {
  read();
  auto op = std::get<Op>(input["root"]);

  input.erase("humn");

  /*
  auto A = eval(op.a);
  auto B = eval(op.b);
  if (A.has_value())
    cout << A.value();
  else
    cout << op.a;
  cout << ' ' << (char)op.opt << ' ';
  if (B.has_value())
    cout << B.value();
  else
    cout << op.b;
  cout << endl;
  */
  //cout << guestimate(op.a, op.b, -100000000, 1000000000) << endl;
  auto ans = eval2(op.a, {eval(op.b).value(), 1});
  cout << ans.first / ans.second << endl;
}

int main()
{
  auto start = chrono::high_resolution_clock::now();
  //first();
  second();
  cout << "Time elapsed: "
       << chrono::duration_cast<chrono::milliseconds>(
              chrono::high_resolution_clock::now() - start)
              .count()
       << "ms" << endl;
}