A = {
    0: [1, 2, 3],
    1: [0, 2, 4, 5],
    2: [0, 1, 6],
    3: [0],
    4: [1],
    5: [1],
    6: [2]
}

def bfs(A):
    """큐를 이용하여 BFS를 구현하여 그래프의 모든 노드를 방문하는 함수를 작성하였다.
    시작 노드 0번부터 가까운 노드들을 우선적으로 방문하며 같은 레벨 내에서는
    번호가 작은 노드부터 우선적으로 탐색을 수행한다.
    방문이 끝난 노드들은 answer 리스트에 순서대로 담겨서 리턴된다.
    """
    answer = []
    visited = [False] * len(A)
    queue = [0] #0번 노드부터 탐색 시작
    visited[0] = True #0번 노드는 탐색 완료

    while queue:
        current = queue.pop(0) #큐의 맨 앞 노드를 꺼내서 먼저 들어온 것부터 처리
        answer.append(current) #꺼낸 노드를 리스트에 추가

        for neighbor in A[current]: #현재 노드와 연결된 이웃 노드를 확인
            #방문하지 않은 노드를 확인하면 큐에 추가
            if not visited[neighbor]:
                queue.append(neighbor)
                visited[neighbor] = True

    return answer


def dfs(A):
    """스택을 이용하여 DFS를 구현하여 그래프의 모든 노드를 방문하는 함수를 작성하였다.
    시작 노드 0번부터 한 방향으로 갈 수 있는 가장 깊은 곳까지 우선적으로 탐색하며
    더 이상 깊이 갈 수 없으면 되돌아와 다음 경로를 탐색한다.
    이웃 노드들을 역순으로 스택에 넣어 번호가 작은 노드부터 우선적으로 탐색을 수행한다.
    방문이 끝난 노드들은 answer 리스트에 순서대로 담겨서 리턴된다.
    """
    answer = []
    visited = [False] * len(A)
    stack = [0] #0번 노드부터 탐색 시작하며 스택에 넣기
    visited[0] = True  #0번 노드는 탐색 완료

    while stack:
        current = stack.pop() #가장 최근에 스택에 추가된 것을 꺼내가
        answer.append(current) #꺼낸 노드를 리스트에 추가

        for neighbor in reversed(A[current]): #노드를 역순 확인하기 (역순 확인을 해야 작은 번호가 먼저 스택에 쌓여서 먼저 꺼내질 수 있게 하기 위함)
            #방문하지 않은 노드를 스택에 추가
            if not visited[neighbor]:
                stack.append(neighbor)
                visited[neighbor] = True 

    return answer

# 아래는 체크함수입니다. 수정하실 필요 없습니다.
bfs_result = bfs(A)
dfs_result = dfs(A)

assert bfs_result == [0,1,2,3,4,5,6]
assert dfs_result == [0,1,4,5,2,6,3]

print('PASSED!')