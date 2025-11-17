/* 프로그램: 여러 학생의 수강 과목 수, 성적 정보를 입력받아 평점평균, 장학금 수혜대상자를 정리하여 보여주는 프로그램입니다.
작성자: 기계정보공학과 2022430008 김재민
작성일: 2025-11-13
고찰: 비정형 배열에 있어서 열의 범위가 변함에 따라 반복문의 조건식도 변하는 것을 생각해내기 어려운 부분이 있었다. 여러 함수를 써 메인함수에서의 계산을 줄이고 싶었지만, 한 반복문에 연계되는 실행문이 많아
여러 함수를 쓰지는 못하였다. 최적화를 더 할 수 있을 것같은 생각이 들어 아쉬움이 든다.
*/

#include <stdio.h>
#include <stdlib.h>                                                // 동적 메모리 함수, atoi함수 등을 쓰기 위해 사용.

double Grade(char grade);                                          // 함수 원형 선언.

int main(int argc, char *argv[])
{
    int num_subject;

    if (argc < 2){                                                // 커맨더라인 입력 오류 처리입니다.
        printf("학생수를 입력하지 않았습니다.\n");
        exit(1);
    }
    int n_students = atoi(argv[1]);                             // atoi함수를 통해 학생 수를 선언 및 초기화한다.

    if (n_students < 1){                                        // 입력받은 학생 수에 대한 오류처리.
        printf("학생 수는 1보다 커야합니다.\n");
        return -1;
    }

    printf("학생별 정보를 입력하세요.\n");

    char **students_info = (char**)malloc(sizeof(char*) * n_students);              // 학생 정보에 대한 동적 메모리 할당. 아래에 내부적인 동적메모리할당이 있음.
    if(students_info == NULL){                                                      // 메모리 할당시 반환값검사.
        printf("동적 메모리 할당에 실패했습니다.\n");
        exit(1);
    }

    for (int i = 0; i < n_students; i++){
        scanf("%d", &num_subject);                                                  // 과목 수를 입력받음.

        if (num_subject < 1){                                                       // 입력받은 과목 수에 대한 오류처리.
            printf("학생의 과목수는 1보다 커야합니다.\n");
            return -2;
        }

        students_info[i] = (char*)malloc(sizeof(char) * (num_subject));             // 과목수(열)에 대한 정보를 입력받았으므로 동적 메모리 할당이 가능해짐.
        if(students_info[i] == NULL){
        printf("동적 메모리 할당에 실패했습니다.\n");
        exit(1);
        }

        for (int j = 0; j < num_subject; j++){                                              // 비정형 배열의 원소값을 입력함.
            scanf(" %c", &students_info[i][j]);
        }

        students_info[i] = (char*)realloc(students_info[i], (num_subject + 1)*sizeof(char));       
        for (int j = 0; j < num_subject+1; j++){
            if (j == num_subject){                                                              // 입력받은 비정형 배열의 크기를 1만큼 늘려주고 마지막에 'X'라는 끝표시를 해준다.
                students_info[i][j] = 'X';
            }
        }
    }

// 출력
    double High_GPA = 0.0;                                      // 학생간 GPA 비교를 위한 변수값.
    int scholar_Index = 0;                                      // 장학금을 누가 받을지 결정해주는 변수.

    for (int i = 0; i < n_students; i++){

        double sum = 0.0, GPA;
        int Subject_count = 0;

        printf("Student %d:", i+1);                               // 몇번 학생인지 출력함.
        for (int j = 0; students_info[i][j] != 'X'; j++){         // 각 행에 대해서 열이 'X' 전에 도착하면 각 학생의 과목을 출력함.
            printf(" %c", students_info[i][j]);                   

            sum += Grade(students_info[i][j]);                    // GPA를 구하기 위한 로직이다. sum에 각 과목의 성적을 더하기를 반복하고, 과목수를 카운팅한다.
            Subject_count++;
        }

        GPA = sum / Subject_count;                                // 반복문을 나온 sum과 과목수 카운팅을 이용해 GPA를 구하고 출력함.
        printf(" ; average GPA: %.2lf", GPA);
        printf("\n");

        if (Subject_count >= 5 && GPA > High_GPA){               // 장학금 수혜자까지 한 반복문으로 한번에 구한다. 과목수가 5이상이고, 최대 GPA를 넘으면
            High_GPA = GPA;                                         // 최대 GPA를 갱신하고 scholar_Index에 학생의 번호를 저장한다.
            scholar_Index = i;
        }
    }

    if(scholar_Index == 0) {                                        // 인덱스가 0이면 출력x.
        printf("Scholarship: \n");
    }
    if (scholar_Index != 0){                                        //인덱스가 값을 가지면 그 인덱스를 출력함.
        printf("Student %d\n", scholar_Index);
    }


    for (int i = 0; i < n_students; i++){                           // 비정형 배열의 동적 메모리 반환(할당 순선의 역순으로)
        free(students_info[i]);
    }
    free(students_info);
    return 0;
}

double Grade(char grade)                                            // 성적을 스코어 형태로 변환시켜주는 함수이다.
{
    if (grade == 'A') return 4.0;
    if (grade == 'B') return 3.0;
    if (grade == 'C') return 2.0;
    if (grade == 'D') return 1.0;
    if (grade == 'F') return 0.0;
}
