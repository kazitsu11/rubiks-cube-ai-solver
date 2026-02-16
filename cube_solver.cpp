#include <bits/stdc++.h>
using namespace std;

struct Cube
{
    int pos[8];
    int ori[8];

    bool operator==(const Cube &other) const
    {
        for (int i = 0; i < 8; i++)
            if (pos[i] != other.pos[i] || ori[i] != other.ori[i])
                return false;
        return true;
    }
};

Cube goal = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {0, 0, 0, 0, 0, 0, 0, 0}};

Cube moveR(const Cube &c)
{
    Cube n = c;
    int p[4] = {0, 3, 7, 4};
    int o[4] = {2, 1, 2, 1};

    for (int i = 0; i < 4; i++)
    {
        n.pos[p[(i + 1) % 4]] = c.pos[p[i]];
        n.ori[p[(i + 1) % 4]] = (c.ori[p[i]] + o[i]) % 3;
    }
    return n;
}

Cube moveU(const Cube &c)
{
    Cube n = c;
    int p[4] = {0, 1, 2, 3};

    for (int i = 0; i < 4; i++)
    {
        n.pos[p[(i + 1) % 4]] = c.pos[p[i]];
        n.ori[p[(i + 1) % 4]] = c.ori[p[i]];
    }
    return n;
}

Cube moveF(const Cube &c)
{
    Cube n = c;
    int p[4] = {0, 4, 5, 1};
    int o[4] = {1, 2, 1, 2};

    for (int i = 0; i < 4; i++)
    {
        n.pos[p[(i + 1) % 4]] = c.pos[p[i]];
        n.ori[p[(i + 1) % 4]] = (c.ori[p[i]] + o[i]) % 3;
    }
    return n;
}

int heuristic(const Cube &c)
{
    int h = 0;
    for (int i = 0; i < 8; i++)
    {
        if (c.pos[i] != i || c.ori[i] != 0)
            h++;
    }
    return h / 4;
}

bool dls(Cube c, int depth)
{
    if (c == goal)
        return true;
    if (depth == 0)
        return false;

    if (dls(moveR(c), depth - 1))
        return true;
    if (dls(moveU(c), depth - 1))
        return true;
    if (dls(moveF(c), depth - 1))
        return true;

    return false;
}

void iddfs(Cube start)
{
    for (int depth = 0; depth <= 10; depth++)
    {
        if (dls(start, depth))
        {
            cout << "solved using IDDFS at depth " << depth << endl;
            return;
        }
    }
}

struct Node
{
    Cube c;
    int g, f;
};

struct cmp
{
    bool operator()(Node a, Node b)
    {
        return a.f > b.f;
    }
};

void astar(Cube start)
{
    priority_queue<Node, vector<Node>, cmp> pq;
    pq.push({start, 0, heuristic(start)});

    while (!pq.empty())
    {
        Node cur = pq.top();
        pq.pop();
        if (cur.c == goal)
        {
            cout << "Solved using A*" << endl;
            return;
        }
        pq.push({moveR(cur.c), cur.g + 1, cur.g + 1 + heuristic(moveR(cur.c))});
        pq.push({moveU(cur.c), cur.g + 1, cur.g + 1 + heuristic(moveR(cur.c))});
        pq.push({moveF(cur.c), cur.g + 1, cur.g + 1 + heuristic(moveR(cur.c))});
    }
}

int idaSearch(Cube c, int g, int threshold)
{
    int f = g + heuristic(c);
    if (f > threshold)
        return f;
    if (c == goal)
        return -1;

    int minNext = INT_MAX;
    vector<Cube> next = {
        moveR(c), moveU(c), moveF(c)};

    for (auto &n : next)
    {
        int t = idaSearch(n, g + 1, threshold);
        if (t == -1)
            return -1;
        minNext = min(minNext, t);
    }
    return minNext;
}

void idastar(Cube start)
{
    int threshold = heuristic(start);

    while (true)
    {
        int t = idaSearch(start, 0, threshold);
        if (t == -1)
        {
            cout << "solved using Korf's IDA*" << endl;
            return;
        }
        threshold = t;
    }
}

int main()
{
    Cube start = goal;

    start = moveR(start);
    start = moveU(start);
    start = moveF(start);

    iddfs(start);
    astar(start);
    idastar(start);

    return 0;
}