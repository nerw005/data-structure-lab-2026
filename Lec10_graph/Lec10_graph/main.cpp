#include "AdjMatGraph.h"
#include <cstdio>

int main()
{
    AdjMatGraph g;

    for (int i = 0; i < 4; i++)
        g.insertVertex('A' + i);  // 들여쓰기 수정
    g.insertEdge(0, 1);
    g.insertEdge(0, 3);
    g.insertEdge(1, 2);
    g.insertEdge(1, 3);
    g.insertEdge(2, 3);
    printf("인접 행렬로 표현한 그래프\n");
    g.display();

    return 0;  // 추가
}