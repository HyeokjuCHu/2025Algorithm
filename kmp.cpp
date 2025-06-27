#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 부분 일치 테이블(Pi 배열)을 계산하는 함수
vector<int> getPi(string p) {
  int m = (int)p.size(), j = 0;
  vector<int> pi(m, 0); // Pi 배열 초기화

  // 패턴의 각 문자를 순회하며 Pi 배열 채우기
  for (int i = 1; i < m; i++) {
    // 일치하지 않으면 이전 접미사로 이동 (Pi 배열 활용)
    while (j > 0 && p[i] != p[j]) {
      j = pi[j - 1];
    }
    // 일치하면 j 증가시키고 Pi 배열에 저장
    if (p[i] == p[j]) {
      pi[i] = ++j;
    }
  }
  return pi;
}

// KMP (Knuth-Morris-Pratt) 알고리즘을 사용하여 문자열 검색
vector<int> kmp(string s, string p) {
  vector<int> ans;
  auto pi = getPi(p); // 부분 일치 테이블 생성

  int n = (int)s.size(), m = (int)p.size(), j = 0; // 텍스트, 패턴 길이, 현재 일치 길이

  // 텍스트의 각 문자를 순회하며 패턴과 비교
  for (int i = 0; i < n; i++) {
    // 일치하지 않으면 이전 접미사로 이동 (Pi 배열 활용)
    while (j > 0 && s[i] != p[j]) {
      j = pi[j - 1];
    }
    // 일치하면 j 증가
    if (s[i] == p[j]) {
      // 패턴 전체가 일치하면 결과에 추가하고, 다음 검색을 위해 j 이동
      if (j == m - 1) {
        ans.push_back(i - m + 1); // 일치하는 시작 인덱스 저장
        j = pi[j];               // 다음 일치를 위해 Pi 배열 값으로 j 업데이트
      } else {
        j++; // 아직 패턴 전체가 일치하지 않았으므로 j 증가
      }
    }
  }
  return ans;
}

int main() {
  string s, p;
  getline(cin, s); // 텍스트 입력
  getline(cin, p); // 패턴 입력

  auto matched = kmp(s, p); // KMP 알고리즘으로 매칭되는 모든 위치 찾기

  printf("%d\n", (int)matched.size()); // 찾은 매칭 개수 출력
  for (auto i : matched) {
    printf("%d ", i + 1); // 1-기반 인덱스로 출력
  }
  printf("\n"); // 마지막에 줄바꿈 추가 (옵션)

  return 0;
}
