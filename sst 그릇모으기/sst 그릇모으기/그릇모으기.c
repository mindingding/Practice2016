#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

int container();
int count;

int main(void) {
	/* �Ʒ� freopen �Լ��� sample_input.txt�� read only �������� ����, ǥ���Է�(Ű����) ��� sample_input.txt �� ���� �о���ڴٴ� �ǹ��� �ڵ��Դϴ�.
	���� ���� PC ���� �׽�Ʈ �� ����, �Է°��� sample_input.txt�� ������ �� freopen �Լ��� ����ϸ�
	�� �Ʒ����� scanf �Լ��� ����Ͽ� ǥ���Է� ��� sample_input.txt ���Ϸ� ���� �Է°��� �о� �� �� �ֽ��ϴ�.
	����, ���� PC���� freopen �Լ��� ������� �ʰ� ǥ���Է��� ����Ͽ� �׽�Ʈ�ϼŵ� �����մϴ�.
	��, Codeground �ý��ۿ��� "�����ϱ�" �� ������ �ݵ�� freopen �Լ��� ����ų� �ּ�(//) ó�� �ϼž߸� �մϴ�. */
	//freopen("sample_input.txt", "r", stdin);

	/* setbuf �Լ��� ������� ������, ������ ���α׷��� ���� '�ð� �ʰ�'�� ���� ���� �Ǿ��� ��,
	printf�� ����� ������ ä������ �ʰ� '0��'�� �� ���� �ֽ��ϴ�.
	�ð� �ʰ� ������ ����� ��� ������ �ް��� �ϽŴٸ� "setbuf(stdout, NULL);" �� ����Ͻñ� �ٶ��ϴ�. */
	//setbuf(stdout, NULL);
	int T;
	int test_case, result;

	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	scanf("%d", &T);
	for (test_case = 1; test_case <= T; test_case++) {

		// �� �κп��� �˰��� ���α׷��� �ۼ��Ͻʽÿ�. �⺻ ������ �ڵ带 ���� �Ǵ� �����ϰ� ������ �ڵ带 ����ϼŵ� �˴ϴ�.
		result = container();
		// �� �κп��� ������ ����Ͻʽÿ�.
		//printf("#testcase%d\n", test_case);
		//printf("%d\n", result);
	}

	return 0;	// �������� �� �ݵ�� 0�� �����ؾ� �մϴ�.
}

int container(){

	int N, i,j ,temp = 0;
	int **arr;
	int len = 0;
	int *place;

	scanf("%d", &N);

	arr = (int**)malloc(sizeof(int*) * N);
	for (i = 0; i < N; i++) {
		scanf("%d", &temp);
		printf("%d", temp);
		arr[i] = (int*)malloc(sizeof(int) * temp);
		for (j = 0; j < temp; j++)
			scanf("%d", &arr[i][j]);
	}

	

	return 0;
}