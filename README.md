# simultaneous_equation_solver
---C-language version---<br>
<br>
<h1>SeqSolverとは</h1>
コマンドプロンプト上でn元連立一次方程式を解きます。ただし、ver.3.6時点で未知数の個数(n)は10個までと制限しています。
<h1>特長</h1>
係数として、整数以外にも、小数や分数形式での入力も可能です。また、答えは整数形式のほかに、小数であれば分数形式で表示します。
<h1>使い方</h1>
１．SeqSolver_ver3.6.cをコンパイルしてください。<br>
２．係数入力用のinput.txtを用意してください。<br>
３．１で得られた実行可能ファイルを実行してください。<br>
<h1>input.txtの入力方法</h1>
次の3元連立一次方程式を解かせたいとします。<br>
<a href="https://www.codecogs.com/eqnedit.php?latex=\left\{\begin{matrix}&space;-0.5x-\frac{2}{3}y&plus;z=1&space;\\&space;2x-0.25y=0&space;\end{matrix}\right." target="_blank"><img src="https://latex.codecogs.com/gif.latex?\left\{\begin{matrix}&space;-0.5x-\frac{2}{3}y&plus;z=1&space;\\&space;2x-0.25y=0&space;\end{matrix}\right." title="\left\{\begin{matrix} -0.5x-\frac{2}{3}y+z=1 \\ 2x-0.25y=0 \end{matrix}\right." /></a>
係数を、以下の例のように行列形式で入力してください。<br>
※※※行頭に文字Fを置くことによって読み取りを終了するので、必ず最後の行に文字Fを用意してください※※※<br>
input.txt


行頭に文字Fを置くことによって読み取りを終了するので、必ず最後の行に文字Fを用意してください。
<h1>今後の計画</h1>
<ul>
  <li>エラーを例外処理形式に変更する</li>
  <li>複数の連立方程式を一気に解けるように、エラー処理でexitしないなどの変更</li>
  <li>分数形式で、分母分子が大きくなったらオーバーフローを防ぐため浮動小数点数形式に変更する。</li>
</ul>
