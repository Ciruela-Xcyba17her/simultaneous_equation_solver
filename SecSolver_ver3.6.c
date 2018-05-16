#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include<stdbool.h>

#define	VAL_AMT_MAX	11
#define	FILENAME	"input.txt"

void check_same_line(
	signed long long denominator[][VAL_AMT_MAX],
	signed long long numerator[][VAL_AMT_MAX],
	unsigned char *check_line,
	unsigned char check_row
	);
void read_and_check_numbers(
	double v[][VAL_AMT_MAX],
	unsigned char *check_line,
	unsigned char *check_row,
	signed long long denominator[][VAL_AMT_MAX],
	signed long long numerator[][VAL_AMT_MAX],
	bool frac_flag[][VAL_AMT_MAX]
	);
void tofrac(
	double v[][VAL_AMT_MAX],
	signed long long denominator[][VAL_AMT_MAX],
	signed long long numerator[][VAL_AMT_MAX],
	unsigned char check_line,
	unsigned char check_row,
	bool frac_flag[][VAL_AMT_MAX]
	);
void reduction(
	signed long long *denominator,
	signed long long *numerator
	);
void frac_deduction(
	signed long long *pm,
	signed long long *ps,
	signed long long m,
	signed long long s
	);
void reformation(
	signed long long denominator[][VAL_AMT_MAX],
	signed long long numerator[][VAL_AMT_MAX],
	unsigned char row_number[],
	unsigned char check_line,
	unsigned char check_row
	);
void print_seq(
	double v[][VAL_AMT_MAX],
	signed long long denominator[][VAL_AMT_MAX],
	signed long long numerator[][VAL_AMT_MAX],
	bool frac_flag[][VAL_AMT_MAX],
	unsigned char check_line,
	unsigned char check_row
	);
void swap_line(
	signed long long denominator[][VAL_AMT_MAX],
	signed long long numerator[][VAL_AMT_MAX],
	unsigned char check_line,
	unsigned char check_row,
	unsigned char x,
	unsigned char y
	);

void print_error(unsigned char error_flag, unsigned char line);

void equation_deduction(
	signed long long denominator[][VAL_AMT_MAX],
	signed long long numerator[][VAL_AMT_MAX],
	unsigned char line,
	unsigned char check_row
	);
void reform_zero(
	signed long long denominator[][VAL_AMT_MAX],
	signed long long numerator[][VAL_AMT_MAX],
	unsigned char check_line,
	unsigned char check_row
	);
bool check_line_zero(
	signed long long numerator[][VAL_AMT_MAX],
	unsigned char line,
	unsigned char check_row
	);
void check_row_zero(
	signed long long numerator[][VAL_AMT_MAX],
	unsigned char check_line,
	unsigned char check_row
	);
void swap_row(
	signed long long denominator[][VAL_AMT_MAX],
	signed long long numerator[][VAL_AMT_MAX],
	unsigned char row_number[VAL_AMT_MAX],
	unsigned char check_row,
	unsigned char from,
	unsigned char to
	);
void print_solution(
	signed long long denominator[][VAL_AMT_MAX],
	signed long long numerator[][VAL_AMT_MAX],
	unsigned char row_number[VAL_AMT_MAX],
	unsigned char check_row,
	unsigned char check_line
	);

signed long long Euclidian_algorithm(signed long long a, signed long long b);


/*
主な変数
frac_flag.......各係数について、分数入力であればtrue、整数・小数入力ならfalse
check_line......係数行列の行数
check_row.......係数行列の列数
row_number......列交換が必要なとき、その列に割り当てられている変数を記憶するため
denominator.....分母
numerator.......分子
v...............係数読み取り時、分数以外の係数を格納
end_flag........Enterを押して終了する機能に使用
*/
void main(){
	bool frac_flag[VAL_AMT_MAX][VAL_AMT_MAX] = { { false } };
	unsigned char i,
		check_line,
		check_row,
		row_number[VAL_AMT_MAX];
	signed long long denominator[VAL_AMT_MAX][VAL_AMT_MAX],
		numerator[VAL_AMT_MAX][VAL_AMT_MAX];
	double v[VAL_AMT_MAX][VAL_AMT_MAX];
	unsigned char end_flag;

	/*式の読み込み*/
	read_and_check_numbers(v, &check_line, &check_row, denominator, numerator, frac_flag);

	/*連立方程式問題表示*/
	printf("入力された連立方程式は以下の通りです。\n");
	print_seq(v, denominator, numerator, frac_flag, check_line, check_row);

	/*列番号の初期化(値は除く)*/
	for (i = 0; i<check_row - 1; i++){
		row_number[i] = i;
	}

	/*入力した係数を分数にする*/
	tofrac(v, denominator, numerator, check_line, check_row, frac_flag);

	/*同じ式・0の入力行の抹消*/
	check_same_line(denominator, numerator, &check_line, check_row);

	/*列が一様に０になっていないか確認*/
	check_row_zero(numerator, check_line, check_row);

	/*式をかんたんにする*/
	for (i = 0; i<check_line; i++){
		equation_deduction(denominator, numerator, i, check_row);
	}

	/*対角行列成分のみ残す*/
	reformation(denominator, numerator, row_number, check_line, check_row);
	reform_zero(denominator, numerator, check_line, check_row);

	/*解の表示*/
	printf("上記の連立方程式の解は以下の通りです。\n");
	print_solution(denominator, numerator, row_number, check_row, check_line);

	printf("プログラムを終了するにはEnterキーを押してください。");
	scanf("%c", &end_flag);
}



/*ファイルから係数を読み取る*/
void read_and_check_numbers(double v[][VAL_AMT_MAX], unsigned char *check_line, unsigned char *check_row, signed long long denominator[VAL_AMT_MAX][VAL_AMT_MAX], signed long long numerator[VAL_AMT_MAX][VAL_AMT_MAX], bool frac_flag[][VAL_AMT_MAX]){
	FILE	*fpr;

	char float_jyou = -1;
	bool float_flag = false, minus_flag = false, num_flag = false, first_zero_flag = false;
	unsigned char line = 0, row = 0, cnt = 0, end_flag, c, c_1prev = ' ', c_2prev = ' ';
	double num = 0.0;

	fpr = fopen(FILENAME, "r");

	/*ファイル読み込みエラー*/
	if (fpr == NULL){
		printf("---ERROR---\n読み込み用ファイル\"数値代入用.txt\"が見つかりませんでした。\n\"input.txt\"を作成してください。\n");
		printf("\nプログラムを終了するにはEnterキーを押してください：");
		scanf("%c", &end_flag);
		exit(1);
	}

	/*Fを読み込むまで*/
	while ((c = getc(fpr)) != 'F'){

		/*
		読み込める文字でなければエラー
		入力できるのは空欄・スラッシュ・小数点・マイナス符号のみ
		*/
		if (!('0' <= c&&c <= '9') && !(isspace((int)c)) && c != '/'&&c != '.'&&c != '-'){
			print_error(3, line + 1);
		}

		/*0で始まる数を入れていたらエラー*/
		if ((isspace((int)c_2prev) || c_2prev == '/') && c_1prev == '0' && ('0' <= c&&c <= '9')){
			print_error(0, line + 1);
		}

		/*スラッシュ、ドットから始まっていたらエラー*/
		if (isspace((int)c_1prev) && (c == '/' || c == '.')){
			print_error(0, line + 1);
		}

		/*数字を読み込んだ場合*/
		if ('0' <= c&&c <= '9'){
			cnt++;
			num_flag = true;

			//小数読み込み状態の場合
			if (float_flag){

				num += (c - '0')*pow(10, float_jyou);
				float_jyou--;

				//整数読み込み状態の場合
			}
			else{

				num *= 10;
				num += (c - '0');

			}

		}
		else if (num_flag){
			/*読み込んだものが数値ではなかった場合*/

			//指定桁までに四捨五入
			if (float_flag){
				num = floor(0.5 + num*pow(10, -float_jyou))*pow(10, float_jyou);
			}

			/*小数点を読み込んだ場合*/
			if (c == '.'){

				//小数点を重ねたらエラー
				if (float_flag){
					print_error(4, line + 1);
				}

				//分数の分母・分子を小数としたらエラー
				if (frac_flag[line][row]){
					print_error(10, line + 1);
				}

				//小数読み取り状態に移行
				float_flag = true;
				float_jyou = -1;

				/*読み込んだ文字が数値ではなく、かつ分数読み取り状態ではない場合*/
			}
			else if (frac_flag[line][row] == 0){

				/*分数記号を読み取った場合*/
				if (c == '/'){

					//分数読み取り状態に移動
					frac_flag[line][row] = 1;

					//スラッシュまで読み込んでいるので、分子は決定
					if (!minus_flag){
						numerator[line][row] = (signed long long)num;
					}
					else{
						numerator[line][row] = (signed long long)(-num);
					}

					minus_flag = false;

				}
				else{
					/*空白を読み取った場合*/

					//係数が小数であったとして値を決定
					if (!minus_flag){
						v[line][row] = num;
					}
					else{
						v[line][row] = -num;
						minus_flag = false;
					}

					float_flag = false;
					row++;

				}

				num = 0.0;
				num_flag = 0;

				/*分数読み取り状態である場合*/
			}
			else if (frac_flag[line][row] == 1){

				//スラッシュを重ねたらエラー
				if (c == '/'){
					print_error(5, line + 1);
				}

				//分数の分母・分子を小数としたらエラー
				if (c == '.'){
					print_error(10, line + 1);
				}

				//空欄を読み込んでいるので、分母を決定できる
				//マイナスは分子につける
				if (minus_flag){
					denominator[line][row] = (signed long long)(-num);
				}
				else{
					denominator[line][row] = (signed long long)num;
				}

				//分母は0にできない
				if (denominator[line][row] == 0){
					print_error(6, line + 1);
				}

				//数値読み取り状態を脱出
				num_flag = false;
				num = 0.0;
				row++;
			}

			/*マイナスを読み込んだ場合*/
		}
		else if (c == '-'){

			minus_flag = true;

		}

		/*改行を読み込んだ場合*/
		if (c == '\n'){

			/*１行の要素数が異ならないか検査*/
			if (line == 0){
				*check_row = row;
			}
			else if (*check_row != row){
				print_error(0, line + 1);
			}

			/*変数の個数の上限を超えるとエラー*/
			if (row >= VAL_AMT_MAX + 1 || line >= VAL_AMT_MAX-1){
				print_error(11, 0);
			}

			line++;
			row = 0;

		}

		/*１，２文字前の文字を記憶*/
		c_2prev = c_1prev;
		c_1prev = c;
	}

	/*未記入はエラー*/
	if (cnt == 0){
		print_error(7, 0);
	}

	/*行数を確定*/
	*check_line = line;

	fclose(fpr);

}



/*すべての係数を分数に変える関数*/
void tofrac(double v[][VAL_AMT_MAX], signed long long denominator[][VAL_AMT_MAX], signed long long numerator[][VAL_AMT_MAX], unsigned char check_line, unsigned char check_row, bool frac_flag[][VAL_AMT_MAX]){

	bool minus_flag = false;
	unsigned char i, j, keta = 0;
	double value;

	/*全ての係数を、b/(10^a)の形にし、約分する*/
	for (i = 0; i<check_line; i++, keta = 0, minus_flag = false){
		for (j = 0; j<check_row; j++, keta = 0, minus_flag = false){

			/*(i,j)要素が分数入力でない場合、分数に変換*/
			if (!frac_flag[i][j]){

				//元値を代入
				value = v[i][j];

				/*係数が0ではない場合*/
				if (value != 0){

					if (value<0){
						minus_flag = true;
						value *= -1;
					}

					//value*(10^keta)が整数になるまでketa++;
					while (!(value >= 1.0&&value - (double)floor(value) <= 0.000001)){
						value *= 10;
						keta++;
					}

					if (minus_flag){
						numerator[i][j] = (signed long long)value*(-1);
					}
					else{
						numerator[i][j] = (signed long long)value;
					}

					denominator[i][j] = (signed long long)(pow(10, keta));

					reduction(&denominator[i][j], &numerator[i][j]);

					/*係数が0である場合*/
				}
				else{

					numerator[i][j] = 0;
					denominator[i][j] = 1;

				}

			}

		}
	}

}



/*同じ式の抹消*/
void check_same_line(signed long long denominator[VAL_AMT_MAX][VAL_AMT_MAX], signed long long numerator[VAL_AMT_MAX][VAL_AMT_MAX], unsigned char *check_line, unsigned char check_row){
	unsigned char i, j, k, l;

	//零の列の抹消
	for (i = 0; i<*check_line; i++){
		if (check_line_zero(numerator, i, check_row)){
			for (l = i; l<*check_line - 1; l++){
				swap_line(denominator, numerator, *check_line, check_row, l, l + 1);
			}
			*check_line -= 1;
		}
	}

	/*i行とj行の組み合わせ*/
	for (i = 0; i<*check_line - 1; i++){
		for (j = i + 1; j<*check_line; j++){

			for (k = 0; k<check_row&&denominator[i][k] == denominator[j][k] && numerator[i][k] == numerator[j][k]; k++){
			}

			//同じ行の組が見つかれば、一つを最終行に移し、行数カウントを減らす
			if (k == check_row){
				for (l = j; l<*check_line - 1; l++){
					swap_line(denominator, numerator, *check_line, check_row, l, l + 1);
				}
				*check_line -= 1;
			}

		}
	}
}



/*行基本変形：行の交換*/
void swap_line(signed long long denominator[][VAL_AMT_MAX], signed long long numerator[][VAL_AMT_MAX], unsigned char check_line, unsigned char check_row, unsigned char x, unsigned char y){
	signed long long tmp;
	unsigned char j;

	for (j = 0; j<check_row; j++){
		tmp = numerator[x][j];
		numerator[x][j] = numerator[y][j];
		numerator[y][j] = tmp;

		tmp = denominator[x][j];
		denominator[x][j] = denominator[y][j];
		denominator[y][j] = tmp;
	}
}



/*対角成分のみ残す*/
void reformation(signed long long denominator[][VAL_AMT_MAX], signed long long numerator[][VAL_AMT_MAX], unsigned char row_number[], unsigned char check_line, unsigned char check_row){
	unsigned char i, j, k, cnt = 1;
	signed long long coefficient_s, coefficient_m;

	for (i = 0; i<check_line; i++){

		/*0の行については何も行わない*/
		if (!check_line_zero(numerator, i, check_row)){

			/*(i,i)成分が0の場合、行・列交換*/
			if (numerator[i][i] == 0){

				for (j = i + 1; numerator[i][i] == 0; j++){

					//(j,i)成分が0でなければ、行交換をする
					if (numerator[j][i] != 0 && j<check_line){
						swap_line(denominator, numerator, check_line, check_row, i, j);
						break;
					}

					//行交換で対処しきれなければ列交換をする,cntは交換する列の間隔
					if (j >= check_line - 1){
						swap_row(denominator, numerator, row_number, check_row, i, i + cnt);
						j = i - 1;
						cnt++;
					}

				}
				cnt = 1;	//交換する列間隔を1にもどす
			}
			
			/*対角行列作成*/
			for (j = 0; j<check_line; j++){
				
				if (i != j){
					coefficient_s = denominator[i][i] * numerator[j][i];
					coefficient_m = numerator[i][i] * denominator[j][i];

					reduction(&coefficient_m, &coefficient_s);

					for (k = 0; k<check_row; k++){
						frac_deduction(&denominator[j][k], &numerator[j][k], coefficient_m*denominator[i][k], coefficient_s*numerator[i][k]);
						reduction(&denominator[j][k], &numerator[j][k]);
					}
				}

				reform_zero(denominator, numerator, check_line, check_row);

			}

		}

		//生成された0列を削除
		check_same_line(denominator, numerator, &check_line, check_row);
	}

}



/*分数の引き算*/
void frac_deduction(signed long long *pm, signed long long *ps, signed long long m, signed long long s){
	*ps = *ps*m - *pm*s;
	*pm = *pm*m;

	reduction(pm, ps);
}



/*連立方程式表示*/
void print_seq(double v[][VAL_AMT_MAX], signed long long denominator[][VAL_AMT_MAX], signed long long numerator[][VAL_AMT_MAX], bool frac_flag[][VAL_AMT_MAX], unsigned char check_line, unsigned char check_row){
	unsigned char i, j;

	for (i = 0; i<check_line; i++){

		printf("{");

		for (j = 0; j<check_row; j++){

			if (frac_flag[i][j] == 0){
				/*分数ではない係数の表示について*/

				if (j == check_row - 1){
					printf("=");
				}
				else if (v[i][j] >= 0 && j != 0){
					printf("+");
				}

				if (fabs((signed long long)v[i][j] - v[i][j])<0.00000001){
					printf("%lld", (signed long long)v[i][j]);
				}
				else{
					printf("%lf", v[i][j]);
				}

			}
			else{
				/*分数の係数の表示について*/

				if (j == check_row - 1){
					printf("=");
				}
				else if (numerator[i][j]>0 && j>0){
					printf("+");
				}

				if (numerator[i][j]>0){
					printf("(%lld/%lld)", numerator[i][j], denominator[i][j]);
				}
				else{
					printf("-(%lld/%lld)", (-1)*numerator[i][j], denominator[i][j]);
				}

			}

			//変数名表示
			if (j != check_row - 1){
				printf("%c", 'A' + j);
			}

		}

		printf("\n");

	}

}



/*連立方程式解表示*/
void print_solution(signed long long denominator[][VAL_AMT_MAX], signed long long numerator[][VAL_AMT_MAX], unsigned char row_number[], unsigned char check_row, unsigned char check_line){
	unsigned char i, j, cnt1 = 0, cnt2 = 0, cnt_num, kai_line, check_atai = 0, cnt_first = 1;
	signed long long ans_denominator, ans_numerator, ans_lld;



	kai_line = check_line - 1;
	for (; check_line_zero(numerator, kai_line, check_row); kai_line--){
	}
	kai_line++;

	for (i = 0; i<check_row - 1; i++, cnt_first = 1){

		printf("%c=", 'A' + i);

		if (i == row_number[cnt1] && cnt1<check_line){

			if (check_line_zero(numerator, cnt1, check_row)){

				printf("%c", 'a' + cnt2);
				cnt1++;
				cnt2++;

			}
			else{

				//文字付き
				for (j = kai_line, cnt_num = 0; j<check_row - 1; j++){

					if (numerator[cnt1][j] != 0){
						ans_numerator = -numerator[cnt1][j] * denominator[cnt1][cnt1];
						ans_denominator = denominator[cnt1][j] * numerator[cnt1][cnt1];
						reduction(&ans_denominator, &ans_numerator);

						if (ans_numerator%ans_denominator == 0){
							/*整数表示の場合*/

							ans_lld = ans_numerator / ans_denominator;

							if (llabs(ans_lld) != 1){
								if (cnt_first == 0){
									printf("%+lld", ans_lld);
								}
								else{
									printf("%lld", ans_lld);
								}
							}
							else{
								if (ans_numerator<0){
									putchar('-');
								}
								else if (cnt_first == 0){
									putchar('+');
								}
							}

						}
						else{
							/*分数表示の場合*/

							if (ans_numerator >= 0){
								if (cnt_first == 0){
									putchar('+');
								}
							}
							else{
								putchar('-');
							}
							printf("(%lld/%lld)", llabs(ans_numerator), ans_denominator);
						}

						printf("%c", 'a' + cnt_num);
						cnt_first = 0;

					}
					cnt_num++;

				}

				if (numerator[cnt1][check_row - 1] != 0){

					ans_numerator = numerator[cnt1][check_row - 1] * denominator[cnt1][cnt1];
					ans_denominator = denominator[cnt1][check_row - 1] * numerator[cnt1][cnt1];
					reduction(&ans_denominator, &ans_numerator);

					if (ans_numerator<0){
						putchar('-');
					}
					else if (cnt_first == 0){
						putchar('+');
					}

					if (ans_numerator%ans_denominator == 0){
						ans_lld = ans_numerator / ans_denominator;
						printf("%lld", llabs(ans_lld));
					}
					else{
						printf("(%lld/%lld)", llabs(ans_numerator), ans_denominator);
					}

				}
				else if (cnt_first == 1){
					putchar('0');
				}

			}

			putchar('\n');
			cnt1++;

		}
		else{

			printf("%c\n", 'a' + cnt2);
			cnt2++;

		}
	}

	//任意の実数の使用を示す
	if (cnt2 != 0){
		printf("(ただし、");

		for (i = 0; i<cnt2; i++){
			if (i>0){
				if (i >= 1){
					putchar(',');
				}
			}
			putchar('a' + i);
		}

		printf("は任意の実数)\n");
	}
}



/*約分用関数*/
void reduction(signed long long *denominator, signed long long *numerator){
	unsigned char minus_flag = 0;
	signed long long div;

	if (*numerator<0){
		*numerator = -(*numerator);
		minus_flag++;
	}

	if (*denominator<0){
		*denominator = -(*denominator);
		minus_flag++;
	}

	do{
		div = Euclidian_algorithm(*denominator, *numerator);
		*numerator /= div;
		*denominator /= div;
	} while (div != 1);

	if (minus_flag == 1){
		*numerator = -(*numerator);
	}

}



/*行のすべての係数の分母・分子について、それぞれを共通の因数があれば割る。行を整理する*/
void equation_deduction(signed long long denominator[][VAL_AMT_MAX], signed long long numerator[][VAL_AMT_MAX], unsigned char line, unsigned char check_row){
	signed long long j, min_denominator, min_numerator;
	unsigned char i, k;

	/*
	分母、分子の最小値minとして、まずは最初の係数をあてる。
	ただし0は意味がないので除く
	*/
	for (i = 0; numerator[line][i] == 0;i++){
	}
	min_numerator = llabs(numerator[line][i]);
	min_denominator = llabs(denominator[line][i]);

	/*分母、分子の最小値を求める*/
	for (i = i + 1; i<check_row; i++){
		if (min_numerator>llabs(numerator[line][i]) && numerator[line][i] != 0){
			min_numerator = llabs(numerator[line][i]);
		}
		if (min_denominator>llabs(denominator[line][i]) && numerator[line][i] != 0){
			min_denominator = llabs(denominator[line][i]);
		}

	}

	/*全ての分母が互いに素となるよう整理*/
	for (j = min_denominator + 1; j >= 1; j--){

		//全ての分母がjを因数に持っているか調査
		for (k = 0; k<check_row&&denominator[line][k] % j == 0 || numerator[line][k] == 0; k++){
		}

		if (k == check_row){
			for (k = 0; k<check_row; k++){

				//分子が0の係数は除外
				if (numerator[line][k] != 0){
					denominator[line][k] /= j;
				}
			}
		}

	}

	/*全ての分子が互いに素となるよう整理*/
	for (j = min_numerator + 1; j >= 1; j--){

		//全ての分子がjを因数に持っているか調査
		for (k = 0; k<check_row&&numerator[line][k] % j == 0 || numerator[line][k] == 0; k++){
		}

		if (k == check_row){
			for (k = 0; k<check_row; k++){
				if (numerator[line][k] != 0){
					numerator[line][k] /= j;
				}
			}
		}

	}

}



/*"0/n"を"0/1"にする*/
void reform_zero(signed long long denominator[][VAL_AMT_MAX], signed long long numerator[][VAL_AMT_MAX], unsigned char check_line, unsigned char check_row){
	unsigned char i, j;

	for (i = 0; i<check_line; i++){
		for (j = 0; j<check_row; j++){
			if (numerator[i][j] == 0){
				denominator[i][j] = 1;
			}
		}
	}
}



/*行全体が0の場合true,そうでなければfalseを返す。また、解なしの判定。*/
bool check_line_zero(signed long long numerator[][VAL_AMT_MAX], unsigned char line, unsigned char check_row){
	unsigned char i;

	for (i = 0; i<check_row; i++){
		if (numerator[line][i] != 0){
			if (i == check_row - 1){
				print_error(1, 0);	//係数行列第i行が0かつ一次結合の和が!=0 -> エラー（解なし）
			}
			else{
				/*行全体は０ではない*/
				return false;
			}
		}
	}
	/*行全体が０であった*/
	return true;

}



/*列が一様に０である場合エラー*/
void check_row_zero(signed long long numerator[][VAL_AMT_MAX], unsigned char check_line, unsigned char check_row){
	unsigned char i, j;

	for (i = 0; i<check_row - 1; i++){

		for (j = 0; j<check_line&&numerator[j][i] == 0; j++){
		}

		if (j == check_line){
			print_error(9, 0);
		}
	}

}



/*from列とto列に対し列交換を行う*/
void swap_row(signed long long denominator[][VAL_AMT_MAX], signed long long numerator[][VAL_AMT_MAX], unsigned char row_number[VAL_AMT_MAX], unsigned char check_row, unsigned char from, unsigned char to){
	signed long long tmp;
	unsigned char i;

	for (i = 0; i<check_row - 1; i++){
		tmp = numerator[i][from];
		numerator[i][from] = numerator[i][to];
		numerator[i][to] = tmp;

		tmp = denominator[i][from];
		denominator[i][from] = denominator[i][to];
		denominator[i][to] = tmp;
	}

	tmp = (int)row_number[from];
	row_number[from] = row_number[to];
	row_number[to] = (int)tmp;
}



/*最大公約数を求める（ユークリッドの互除法）*/
signed long long Euclidian_algorithm(signed long long a, signed long long b){
	if (b == 0){
		return a;
	}
	else{
		return Euclidian_algorithm(b, a%b);
	}
}



/*エラー処理*/
void print_error(unsigned char error_flag, unsigned char line){
	unsigned char end_switch;
	printf("+-------+\n");
	printf("| ERROR |\n");
	printf("+-------+\n");

	switch (error_flag){
	case	0:
		/*式にミス*/
		printf("\"input.txt\"の入力方法に誤りがあります。（line %d）\n", line);
		break;
	case	1:
		/*解なし*/
		printf("この連立方程式には解はありません。\n");
		break;
	case	2:
		printf("式の個数と変数の個数が不一致です。（line %d）\n", line);
		break;
	case	3:
		printf("使用できない文字が式に含まれています。（line %d）\n", line);
		break;
	case	4:
		printf("式中の小数点の使用箇所についてエラーが検出されました。（line %d）\n", line);
		break;
	case	5:
		printf("式中のスラッシュの使用箇所についてエラーが検出されました。（line %d）\n", line);
		break;
	case	6:
		printf("0を分母として入力できません。（line %d）\n", line);
		break;
	case	7:
		printf("数値代入用.txtが未記入です。\n");
		break;
	case	8:
		printf("解は無数にあります。\n");
		break;
	case	9:
		printf("変数の係数を一様に0に設定することはできません。\n");
		break;
	case	10:
		printf("分数の分母・分子として小数を設定することはできません。（line %d）\n", line);
		break;
	case	11:
		printf("%d個以上の変数を使用することはできません。", VAL_AMT_MAX);
		break;
	}

	printf("\nプログラムを終了するにはEnterキーを押してください：");
	scanf("%c", &end_switch);
	exit(1);
}
