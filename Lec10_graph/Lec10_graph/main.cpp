#define _CRT_SECURE_NO_WARNINGS        // C4996 경고 비활성화
#pragma warning(disable : 6262)        // C6262: 스택 사용량 초과 경고 비활성화
#include <cstdio>
#include "AdjMatGraph.h"               // AdjMatGraph + SrchAMGraph 모두 포함

// =============================================
// 그래프 입출력 테스트
// =============================================
void testAdjMatGraph() {
    AdjMatGraph g;  // 새로운 그래프 객체 생성

    for (int i = 0; i < 4; i++)
        g.insertVertex('A' + i);  // 정점 삽입: 'A', 'B', 'C', 'D'

    // 간선 삽입
    g.insertEdge(0, 1);
    g.insertEdge(0, 3);
    g.insertEdge(1, 2);
    g.insertEdge(1, 3);
    g.insertEdge(2, 3);

    printf("인접 행렬로 표현한 그래프\n");
    g.display();
}

// =============================================
// DFS 탐색 테스트
// =============================================
void testDFS() {
    SrchAMGraph g;  // DFS 탐색기능이 있는 그래프 객체 생성

    g.load("graph.txt");         // 파일 "graph.txt"로부터 g를 설정함
    printf("그래프(graph.txt)\n");
    g.display();                  // 그래프를 화면에 출력

    printf("\nDFS ==> ");
    g.resetVisited();            // 모든 정점을 방문하지 않았다고 하고
    g.DFS(0);                    // 0번째 정점(A)에서 깊이 우선 탐색 시작
    printf("\n");
}

// =============================================
// main 함수
// =============================================
int main() {
    printf("=== 인접 행렬 그래프 테스트 ===\n");
    testAdjMatGraph();

    printf("\n=== DFS 탐색 테스트 ===\n");
    testDFS();

    return 0;
}