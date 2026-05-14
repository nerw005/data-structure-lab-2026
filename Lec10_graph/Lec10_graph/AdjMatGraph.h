#pragma once
#define _CRT_SECURE_NO_WARNINGS        // C4996: fopen, fscanf unsafe 경고 비활성화
#pragma warning(disable : 6031)        // C6031: 반환값 무시 경고 비활성화
#pragma warning(disable : 6262)        // C6262: 스택 사용량 초과 경고 비활성화
#pragma warning(disable : 26495)       // C26495: 멤버 변수 미초기화 경고 비활성화
#include <cstdio>
#include <cstring>

#define MAX_VTXS 256

// =============================================
// AdjMatGraph 클래스: 인접 행렬 기반 그래프
// =============================================
class AdjMatGraph {
protected:
    int  size;
    char vertices[MAX_VTXS];
    int  adj[MAX_VTXS][MAX_VTXS];

public:
    AdjMatGraph() { reset(); }

    char getVertex(int i) { return vertices[i]; }
    int  getEdge(int i, int j) { return adj[i][j]; }
    void setEdge(int i, int j, int val) { adj[i][j] = val; }

    bool isEmpty() { return size == 0; }
    bool isFull() { return size >= MAX_VTXS; }

    // 그래프 초기화
    void reset() {
        size = 0;
        for (int i = 0; i < MAX_VTXS; i++)
            for (int j = 0; j < MAX_VTXS; j++)
                setEdge(i, j, 0);
    }

    // 정점 삽입
    void insertVertex(char name) {
        if (!isFull())
            vertices[size++] = name;
        else
            printf("Error: 그래프 정점 개수 초과\n");
    }

    // 간선 삽입 (무방향 그래프)
    void insertEdge(int u, int v) {
        setEdge(u, v, 1);
        setEdge(v, u, 1); // 방향 그래프에서는 이 줄 제거
    }

    // 그래프 출력 (콘솔 또는 파일)
    void display(FILE* fp = stdout) {
        fprintf(fp, "%d\n", size);          // 정점의 개수 출력
        for (int i = 0; i < size; i++) {    // 각 행의 정보 출력
            fprintf(fp, "%c ", getVertex(i)); // 정점의 이름 출력
            for (int j = 0; j < size; j++)
                fprintf(fp, " %3d", getEdge(i, j));
            fprintf(fp, "\n");
        }
    }

    // 파일에서 그래프 읽기
    void load(const char* filename) {
        FILE* fp = fopen(filename, "r");
        if (fp == nullptr) {
            printf("Error: 파일을 열 수 없습니다 (%s)\n", filename);
            return;
        }
        reset();
        int n = 0;
        (void)fscanf(fp, "%d", &n);
        for (int i = 0; i < n; i++) {
            char name = '\0';
            (void)fscanf(fp, " %c", &name);
            insertVertex(name);
            for (int j = 0; j < n; j++) {
                int val = 0;
                (void)fscanf(fp, "%d", &val);
                setEdge(i, j, val);
            }
        }
        fclose(fp);
    }
};


// =============================================
// SrchAMGraph 클래스: DFS 탐색 기능 추가
// =============================================
class SrchAMGraph : public AdjMatGraph {
    bool visited[MAX_VTXS]; // 정점의 방문 정보

public:
    SrchAMGraph() : AdjMatGraph() {
        memset(visited, 0, sizeof(visited)); // visited 배열 초기화 (C26495 해결)
    }

    // 모든 정점을 방문하지 않았다고 설정
    void resetVisited() {
        for (int i = 0; i < size; i++)
            visited[i] = false;
    }

    bool isLinked(int u, int v) { return getEdge(u, v) != 0; }

    // 깊이 우선 탐색 (DFS)
    void DFS(int v) {
        visited[v] = true;                          // 현재 정점을 방문함
        printf("%c ", getVertex(v));                // 정점의 이름 출력
        for (int w = 0; w < size; w++)
            if (isLinked(v, w) && visited[w] == false)
                DFS(w);                             // 연결 + 방문X => 순환호출로 방문
    }
};
