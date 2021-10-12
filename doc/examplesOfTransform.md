* char

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

char variable-name = 'a';

<code></pre></td>

<td><pre lang="java"><code>


"a" > bindingName  

<code></pre></td>

</tr>
</table>

* Usage

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>
#include <iostream>
using namespace std;
<br>
int main() {
&nbsp;&nbsp;&nbsp;&nbsp;char grade = 'B';
&nbsp;&nbsp;&nbsp;&nbsp;cout << "I scored a: "<<grade;
&nbsp;&nbsp;&nbsp;&nbsp;return 0;
}

<code></pre></td>

<td><pre lang="java"><code>
+package c2eo.types
<br>
+alias stdout org.eolang.io.stdout
+alias sprintf org.eolang.txt.sprintf
<br>
[] > charApp
&nbsp;&nbsp;"B" > grade
&nbsp;&nbsp;stdout > @
&nbsp;&nbsp;&nbsp;&nbsp;sprintf "%s%s" "I scored a: " firstleter.toString

<code></pre></td>

</tr>
</table>

## Realization in EO

<table>
<tr>
<th>int</th>
<th>float, double</th>
</tr>

<tr>
<td><pre lang="java"><code>

+package c2eo.types
&nbsp;
+alias sprintf org.eolang.txt.sprintf
&nbsp;
[] > сInt
&nbsp;
&nbsp;&nbsp;memory > @
&nbsp;
&nbsp;&nbsp;"int" > type
&nbsp;
&nbsp;&nbsp;sprintf "%d" $ > toString

<code></pre></td>

<td><pre lang="java"><code>

+package c2eo.types
&nbsp;
+alias sprintf org.eolang.txt.sprintf
&nbsp;
[] > cFloat
&nbsp;
&nbsp;&nbsp;memory > @
&nbsp;
&nbsp;&nbsp;"float" > type
&nbsp;
&nbsp;&nbsp;sprintf "%f" $ > toString

<code></pre></td>

</tr>
</table>

## Operators

* Assignment operator =

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

float a = 0;
float b = 1;
b = a;

<code></pre></td>

<td><pre lang="java"><code>

cfloat > a
cfloat > b
b.write a

<code></pre></td>

</tr>
</table>

* Addition operator  +

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

float a = 0;
float b = 1;
b = a + b;

<code></pre></td>

<td><pre lang="java"><code>

cfloat > a
cfloat > b
b.write (a.add b)

<code></pre></td>

</tr>
</table>

* Substraction operator -

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

float a = 0;
float b = 1;
b = a - b;

<code></pre></td>

<td><pre lang="java"><code>

cfloat > a
cfloat > b
b.write (a.sub b)

<code></pre></td>

</tr>
</table>

* Multiply operator *

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

float a = 0;
float b = 1;
b = a * b;

<code></pre></td>

<td><pre lang="java"><code>

cfloat > a
cfloat > b
b.write (a.mul b)

<code></pre></td>

</tr>
</table>

* Divide operator /

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

float a = 0;
float b = 1;
b = a / b;

<code></pre></td>

<td><pre lang="java"><code>

cfloat > a
cfloat > b
b.write (a.div b)

<code></pre></td>

</tr>
</table>

* Mod operator %

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

float a = 0;
float b = 1;
b = a / b;

<code></pre></td>

<td><pre lang="java"><code>

cfloat > a
cfloat > b
b.write (a.div b)

<code></pre></td>

</tr>
</table>

* etc.

## Enum type realization in EO

``` java
[] > week

  memory > @

  "enum" > type

  sprintf "%d" $ > toString
```
 
<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

enum week { Mon, Tue, Wed, Thur, Fri, Sat, Sun};
&nbsp;  
int main()
{
&nbsp;&nbsp;enum week day;
&nbsp;&nbsp;day = Wed;
&nbsp;&nbsp;printf("%d",day);
&nbsp;&nbsp;return 0;
} 
&nbsp;  
<code></pre></td>

<td><pre lang="java"><code>

Mon > 1
Tue > 2
...
Sun > 7
&nbsp;
[] > main
&nbsp;&nbsp;enum > day
&nbsp;
&nbsp;&nbsp;seq > @
&nbsp;&nbsp;&nbsp;&nbsp;day.write Wed
&nbsp;&nbsp;&nbsp;&nbsp;stdout
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;sprintf "%d" Wed
<code></pre></td>

</tr>
</table>

## Enum type realization in EO

``` java
[] > week

  memory > @

  "enum" > type

  sprintf "%d" $ > toString
```
 
<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

enum week { Mon, Tue, Wed, Thur, Fri, Sat, Sun};
&nbsp;  
int main()
{
&nbsp;&nbsp;enum week day;
&nbsp;&nbsp;day = Wed;
&nbsp;&nbsp;printf("%d",day);
&nbsp;&nbsp;return 0;
} 
&nbsp;  
<code></pre></td>

<td><pre lang="java"><code>

Mon > 1
Tue > 2
...
Sun > 7
&nbsp;
[] > main
&nbsp;&nbsp;enum > day
&nbsp;
&nbsp;&nbsp;seq > @
&nbsp;&nbsp;&nbsp;&nbsp;day.write Wed
&nbsp;&nbsp;&nbsp;&nbsp;stdout
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;sprintf "%d" Wed
<code></pre></td>

</tr>
</table>

[While not working yet](https://github.com/cqfn/eo/issues/329)

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

for(int i=0; _expression; _statement1)
{
&nbsp;&nbsp;_statement2
}
<code></pre></td>

<td><pre lang="java"><code>

Сan be implemented through using while

<code></pre></td>

</tr>
</table>

* if

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

if ( _expression )
{
&nbsp;&nbsp;_statement
}

<code></pre></td>

<td><pre lang="java"><code>


if._expression
&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;_statement
&nbsp;&nbsp;seq  

<code></pre></td>

</tr>
</table>

* if-else

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

if ( _expression )
{
&nbsp;&nbsp;_statement1
}
else
{
&nbsp;&nbsp;_statement2
}

<code></pre></td>

<td><pre lang="java"><code>

if._expression
&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;_statement1
&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;_statement2

<code></pre></td>

</tr>
</table>

* if-else_if

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>


if ( _expression1 )
{
&nbsp;&nbsp;_statement1
}
else if ( _expression2 )
{
&nbsp;&nbsp;_statement2
}

<code></pre></td>

<td><pre lang="java"><code>

if._expression1
&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;_statement1
&nbsp;&nbsp;if._expression2
&nbsp;&nbsp;&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_statement2
&nbsp;&nbsp;&nbsp;&nbsp;seq

<code></pre></td>

</tr>
</table>

* if-else_if-else

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

if ( _expression1 )
{
&nbsp;&nbsp;_statement1
}
else if ( _expression2 )
{
&nbsp;&nbsp;_statement2 
}
else
{
&nbsp;&nbsp;_statement3
}

<code></pre></td>

<td><pre lang="java"><code>

if._expression1
&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;_statement1
&nbsp;&nbsp;if._expression2
&nbsp;&nbsp;&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_statement2
&nbsp;&nbsp;&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_statement3

<code></pre></td>

</tr>
</table>

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

int f(int a, int b)
{
&nbsp;&nbsp;int с;
&nbsp;&nbsp;c = a + b;
&nbsp;&nbsp;return c;
}

<code></pre></td>

<td><pre lang="java"><code>

[a, b] > f
&nbsp;&nbsp;integer > c
&nbsp;&nbsp;if. > @
&nbsp;&nbsp;&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;c.write (a.add b)
&nbsp;&nbsp;&nbsp;&nbsp;c
&nbsp;&nbsp;&nbsp;&nbsp;seq

<code></pre></td>

</tr>
</table>

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

struct rectangle
{
&nbsp;&nbsp;int x, y;
};

<code></pre></td>

<td><pre lang="java"><code>


[] > rectangle
&nbsp;&nbsp;cInt > x
&nbsp;&nbsp;cInt > y

<code></pre></td>

</tr>
</table>

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

switch ( _expression )
{
&nbsp;&nbsp;case: _value1:
&nbsp;&nbsp;&nbsp;&nbsp;_statement1
&nbsp;&nbsp;&nbsp;&nbsp;break;
&nbsp;
&nbsp;&nbsp;case: _value2:
&nbsp;&nbsp;&nbsp;&nbsp;_statement2
&nbsp;&nbsp;&nbsp;&nbsp;break;
&nbsp;
&nbsp;&nbsp;case: _value3:
&nbsp;&nbsp;&nbsp;&nbsp;_statement3
&nbsp;&nbsp;&nbsp;&nbsp;break;
&nbsp;
&nbsp;&nbsp;case: _default:
&nbsp;&nbsp;&nbsp;&nbsp;_statement4
&nbsp;&nbsp;&nbsp;&nbsp;break;
}

<code></pre></td>

<td><pre lang="java"><code>


if.(_expression.eq _value1)
&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;_statement1
&nbsp;&nbsp;if.(_expression.eq _value2)
&nbsp;&nbsp;&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_statement2
&nbsp;&nbsp;&nbsp;&nbsp;if.(_expression.eq _value3)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_statement3
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;seq
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_statement4

<code></pre></td>

</tr>
</table>

<table>
<tr>
<th>C</th>
<th>EO</th>
</tr>

<tr>
<td><pre lang="c"><code>

typedef struct
{
&nbsp;&nbsp;float due;
&nbsp;&nbsp;int over_due;
} client;
&nbsp;
client a;

<code></pre></td>

<td><pre lang="java"><code>

[] > client 
&nbsp;&nbsp;cFloat > due
&nbsp;&nbsp;cInt > overdue
&nbsp;
client > a
<code></pre></td>

</tr>
</table>

* while, do while
[While not working yet](https://github.com/cqfn/eo/issues/329)