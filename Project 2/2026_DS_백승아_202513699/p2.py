import sys

## 입력 받는 코드입니다. 수정할 필요 없습니다.
sys.stdin = open('case.txt')
N, M = list(map(int,input().split()))
print(N, M)
concerts = []
for v in range(N):
    values = list(map(int, input().split()))
    concerts.append(values)
# print(concerts)
# [[1, 0, 0, 1, 1, 0], [1, 0, 1, 1, 0, 0], [1, 1, 1, 1, 0, 1], [0, 1, 1, 0, 1, 1], [0, 1, 0, 0, 1, 0]]
###################################

def count_stages(concerts):
    """BFS를 이용하여 콘서트장의 무대 공간 개수를 반환하는 함수를 작성함.
    1이 있는 공간은 펜스가 위치한 공간, 0이 있는 공간은 무대가 있는 빈 공간을 나타내고 있어 0이 서로 연결된 공간은 1개의 무대 공간으로 인식함.
    큐를 사용하여 시작지점에서 가까운 칸부터 탐색하는 방식을 사용하며 연결된 0 공간은 전체를 방문 처리하고
    이미 방문한 칸은 즉각적인 방문 표시를 통하여 중복 탐색이 되지 않도록 함.
    전체를 반복하며 방문하지 않은 0을 방문하면 BFS를 실행하여 카운트를 1 올리는 방식을 사용하며
    최종적으로 BFS가 실행된 횟수가 즉 독립적인 무대 공간의 개수가 된다.
    """
    N = len(concerts) #행 개수 선언
    M = len(concerts[0]) #열 개수 선언
    answer = 0
    visited = [[False] * M for _ in range(N)] #처음에는 모두 False로 선언하고 방문 여부를 2차원으로 배열

    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)] #행과 열 변화량 정의

    #DFS가 아닌 BFS사용, 둘 다 시간 복잡도에서는 O로 차이가 없을 것이라고 판단하여 더 편하다고 생각되는 BFS로 구상함.
    def bfs(start_r, start_c):
        #시작 지점을 큐에 넣고 탐색 시작
        queue = [(start_r, start_c)]
        visited[start_r][start_c] = True #시작 지점은 방문으로 표시

        while queue:
            r, c = queue.pop(0) #큐에서 맨 앞 좌표를 꺼냄

            #전방향 확인 후 이동할 다음 좌표 계산
            for dr, dc in directions: 
                nr, nc = r + dr, c + dc

                #만약 범위 안에 있으며 방문을 하지 않고 0인 경우에만 이동을 수행
                if 0 <= nr < N and 0 <= nc < M and not visited[nr][nc] and concerts[nr][nc] == 0:
                    queue.append((nr, nc)) #다음 탐색을 위해 큐에 추가
                    visited[nr][nc] = True #큐에 넣은 대상을 중복 방문하는 것을 방지하기 위하여 방문 표시를 진행

    for r in range(N):
        for c in range(M):
            #방문하지 않은 0을 찾으면 BFS로 0의 범위를 탐색
            if not visited[r][c] and concerts[r][c] == 0: 
                bfs(r, c)
                answer += 1 #무대 공간 수 +1

    return answer

print(count_stages(concerts))