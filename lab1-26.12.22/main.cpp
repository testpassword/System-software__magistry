/*
https://leetcode.com/problems/the-dining-philosophers/

Пусть вилки будут пронумерованы от 0 до 4. Сначала философ берёт вилку с наименьшим номеров, из лежащих рядом, потом с наибольшим из лежащих рядом.
Получается, что в ситуации, когда 4 философа взяли по вилке, для последнего философа, вилка с наименьшим номером будет не доступна, 
он не сможет взять ни одну вилку и не устроит всем дедлок. 
Небольшая модификация позволит ускорить алгоритм в ситуации, когда все потоки запустились одновременно и работают известное количество времени. В таком случае, 
можно для каждого нечётного потока поменять приоритет захвата (захватывать сначала ресурс с большим временем, потом с меньшим). Таким образов, как только первый 
поток завершит свою работу и освободит ресурс, запустится смогут оба его соседа.
*/
#include <mutex>
#include <algorithm> // std::swap

class DiningPhilosophers {
    private:
        std::mutex mtxs[5];
    public:
        DiningPhilosophers() {}

        void wantsToEat(
            int id,
            function<void()> pickLFork,
            function<void()> pickRFork,
            function<void()> eat,
            function<void()> putLFork,
            function<void()> putRFork
        ) {
            int first, second = -1;
            auto get_second = [id]() { return (id + 1) % 5; };
            if (id % 2 == 0) first = id, second = get_second();
            else second = id, first = get_second();
            mtxs[first].lock();
            mtxs[second].lock();
            pickLFork();
            pickRFork();
            eat();
            putRFork();
            putLFork();
            mtxs[second].unlock();
            mtxs[first].unlock();
        }
};