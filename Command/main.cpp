#include <string>
#include <iostream>

/**
 * Интерфейс Команды объявляет метод для выполнения команд.
 */
class ICommand {
public:
    virtual ~ICommand() {}
    virtual void Execute() const = 0;
};
/**
 * Некоторые команды способны выполнять простые операции самостоятельно.
 */
class SimpleCommand : public ICommand {
public:
    explicit SimpleCommand(const std::string &payload) : payload(payload) {
    }
    void Execute() const override {
        std::cout << "SimpleCommand: See, I can do simple things like printing (" << this->payload << ")\n";
    }
private:
    std::string payload;
};

/**
 * Классы Получателей содержат некую важную бизнес-логику. Они умеют выполнять
 * все виды операций, связанных с выполнением запроса. Фактически, любой класс
 * может выступать Получателем.
 */
class Receiver {
public:
    void DoSomething(const std::string &a) {
        std::cout << "Receiver: Working on (" << a << ".)\n";
    }
    void DoSomethingElse(const std::string &b) {
        std::cout << "Receiver: Also working on (" << b << ".)\n";
    }
};

/**
 * Но есть и команды, которые делегируют более сложные операции другим объектам,
 * называемым «получателями».
 */
class ComplexCommand : public ICommand {
public:
/**
 * Сложные команды могут принимать один или несколько объектов-получателей
 * вместе с любыми данными о контексте через конструктор.
 */
    ComplexCommand(Receiver &receiver, const std::string &a, const std::string &b)
            : receiver(receiver)
            , a(a)
            , b(b)
    {}
    /**
     * Команды могут делегировать выполнение любым методам получателя.
     */
    void Execute() const override {
        std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
        this->receiver.DoSomething(this->a);
        this->receiver.DoSomethingElse(this->b);
    }
private:
    Receiver &receiver;
    /**
     * Данные о контексте, необходимые для запуска методов получателя.
     */
    std::string a;
    std::string b;
};

/**
 * Отправитель связан с одной или несколькими командами. Он отправляет запрос
 * команде.
 */
class Invoker {
public:
    void SetOnStart(const ICommand &command) {
        this->onStart = &command;
    }
    void SetOnFinish(const ICommand &command) {
        this->onFinish = &command;
    }
    /**
     * Отправитель не зависит от классов конкретных команд и получателей.
     * Отправитель передаёт запрос получателю косвенно, выполняя команду.
     */
    void DoSomethingImportant() {
        std::cout << "Invoker: Does anybody want something done before I begin?\n";
        if (this->onStart) {
            this->onStart->Execute();
        }
        std::cout << "Invoker: ...doing something really important...\n";
        std::cout << "Invoker: Does anybody want something done after I finish?\n";
        if (this->onFinish) {
            this->onFinish->Execute();
        }
    }
private:
    const ICommand* onStart = nullptr;
    const ICommand* onFinish = nullptr;
};
/**
 * Клиентский код может параметризовать отправителя любыми командами.
 */

int main() {
    Invoker invoker;

    SimpleCommand simpleCommand("Say Hi!");
    invoker.SetOnStart(simpleCommand);

    Receiver receiver;
    ComplexCommand complexCommand(receiver, "Send email", "Save report");
    invoker.SetOnFinish(complexCommand);

    invoker.DoSomethingImportant();

    return 0;
}