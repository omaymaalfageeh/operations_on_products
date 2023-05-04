// operation_on_products.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <string>
using namespace std;
struct Product
{
    string name;
    int quantity;
    float price;
};

const int MAX_PRODUCTS = 100; // أقصى عدد من المنتجات المسموح به
Product products[MAX_PRODUCTS]; // مصفوفة تخزن المنتجات
int numProducts = 0; // عدد المنتجات الحالي

struct Operation
{
    int type; // 1: add, 2: remove, 3: sell, 4: buy, 5: modify
    int index; // index of the product in the products array
    Product product;
};

const int MAX_OPERATIONS = 5; // أقصى عدد من العمليات المسموح به
Operation undoStack[MAX_OPERATIONS]; // stack لتخزين العمليات الملغاة
Operation redoStack[MAX_OPERATIONS]; // stack لتخزين العمليات المسترجعة
int numUndo = 0; // عدد العمليات في undo stack
int topundo = -1;
int numRedo = 0; // عدد العمليات في redo stack
int topredo = -1;
int full(int top)
{
    return top == MAX_OPERATIONS;
}
int empty(int top)
{
    return top == -1;
}
void push(Operation a[], int top, Operation op)
{
    if (!full(top))
    {
        top = top + 1;
        a[top] = op;
    }
    else
        cout << "FULL stack" << endl;
}
void pop(int top)
{
    if (!empty(top))
        top = top - 1;
}
int search(Product products[], string name)
{
    for (int i = 0; i < numProducts; i++)
        if (products[i].name == name)
            return i;
    return -1;
}
void addProduct()
{
    if (numProducts == MAX_PRODUCTS)
    {
        cout << "The store is full. Cannot add more products." << endl;
        return;
    }
    else
    {
        string nameadd;
        cout << "Enter product name: ";
        cin >> nameadd;
        if (search(products, nameadd) == -1)
        {
            Product newProduct;
            newProduct.name = nameadd;
            cout << "Enter product quantity: ";
            cin >> newProduct.quantity;
            cout << "Enter product price: ";
            cin >> newProduct.price;
            products[numProducts] = newProduct;
            numProducts++;
            Operation op;
            op.type = 1;
            op.index = numProducts - 1;
            op.product = newProduct;
            push(undoStack, topundo, op);
            numUndo++;
            cout << "Product added successfully." << endl;
        }
        else
            cout << "product found " << endl;
    }
}
void removeProduct()
{
    if (numProducts == 0)
    {
        cout << "The store is empty. Cannot remove products." << endl;
        return;
    }
    string nameremove;
    cout << "Enter the name of the product you want to remove: ";
    cin >> nameremove;
    if (search(products, nameremove) != -1)
    {
        Operation op;
        op.type = 2;
        op.index = search(products, nameremove);
        op.product = products[search(products, nameremove)];
        for (int i = search(products, nameremove); i < numProducts - 1; i++)
        {
            products[i] = products[i + 1];
        }
        numProducts--;
        push(undoStack, topundo, op);
        numUndo++;
        cout << "Product removed successfully." << endl;
    }
    else
        cout << "product not found" << endl;

}

void printProducts()
{
    if (numProducts == 0)
    {
        cout << "The store is empty." << endl;
        return;
    }
    cout << "Index\tName\tQuantity\tPrice" << endl;
    for (int i = 0; i < numProducts; i++)
    {
        cout << i << "\t" << products[i].name << "\t" << products[i].quantity << "\t\t" << products[i].price << endl;
    }
}

void sellProduct()
{
    if (numProducts == 0)
    {
        cout << "The store is empty. Cannot sell products." << endl;
        return;
    }
    string sellname;
    cout << "Enter the name of the product you want to sell: ";
    cin >> sellname;
    int sellindex = search(products, sellname);
    if (sellindex != -1)
    {
        int sellquantity;
        cout << "Enter the quantity you want to sell: ";
        cin >> sellquantity;
        if (sellquantity <= 0 || sellquantity > products[sellindex].quantity)
        {
            cout << "Invalid quantity. Please enter a valid quantity." << endl;
            return;
        }
        Operation op;
        op.type = 3;
        op.index = sellindex;
        op.product = products[sellindex];
        products[sellindex].quantity -= sellquantity;
        push(undoStack, topundo, op);
        numUndo++;
        cout << "Product sold successfully." << endl;
    }
    else
        cout << "product not found" << endl;

}
void buyProduct()
{
    if (numProducts == MAX_PRODUCTS)
    {
        cout << "The store is full. Cannot buy more products." << endl;
        return;
    }
    string buyname;
    cout << "Enter product name: ";
    cin >> buyname;
    int indexbuy = search(products, buyname);
    if (indexbuy != -1)
    {
        Product newProduct;
        newProduct.name = buyname;
        cout << "Enter product quantity: ";
        cin >> newProduct.quantity;
        cout << "Enter product price: ";
        cin >> newProduct.price;
        products[indexbuy].quantity += newProduct.quantity;
        products[indexbuy].price = newProduct.price;
        Operation op;
        op.type = 4;
        op.index = numProducts - 1;
        op.product = newProduct;
        push(undoStack, topundo, op);
        numUndo++;
        cout << "Product bought successfully." << endl;
    }
    else
        cout << "product not found" << endl;
}

void modifyProduct()
{
    if (numProducts == 0)
    {
        cout << "The store is empty. Cannot modify products." << endl;
        return;
    }
    string modifyname;
    cout << "Enter the name of the product you want to modify: ";
    cin >> modifyname;
    int modifyindex = search(products, modifyname);
    if (modifyindex != -1)
    {
        Operation op;
        op.type = 5;
        op.index = modifyindex;
        op.product = products[modifyindex];
        cout << "Enter new quantity: ";
        cin >> products[modifyindex].quantity;
        cout << "Enter new price: ";
        cin >> products[modifyindex].price;
        push(undoStack, topundo, op);
        numUndo++;
        cout << "Product modified successfully." << endl;
    }
    else
        cout << "product not found" << endl;

}
void PrintStack(Operation a[], int top)
{
    for (int i = top; i >= 0; i--)
        cout << a[i].type << "\t" << a[i].product.name << "\t" << a[i].product.price << "\t" << a[i].product.quantity;
}
Operation ret(Operation op[], int top)
{
    Operation opp = op[top];
    return opp;
}
int main()
{
    int choice;
    do
    {
        cout << "Enter your choice:" << endl;
        cout << "1. Add product" << endl;
        cout << "2. Remove product" << endl;
        cout << "3. Sell product" << endl;
        cout << "4. Buy product" << endl;
        cout << "5. Modify product" << endl;
        cout << "6. Print products" << endl;
        cout << "7. Undo" << endl;
        cout << "8. Redo" << endl;
        cout << "9. PrintStack" << endl;
        cout << "10. Exit" << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:
            addProduct();
            break;
        case 2:
            removeProduct();
            break;
        case 3:
            sellProduct();
            break;
        case 4:
            buyProduct();
            break;
        case 5:
            modifyProduct();
            break;
        case 6:
            printProducts();
            break;
            /*   case 7:
            undo();
            break;
              case 8:
                  redo();
                  break;*/
        case 9:
            cout << "Enter 1 to print undoStack || 2 to print redoStack" << endl;
            int p;
            cin >> p;
            switch (p)
            {
            case 1:
                PrintStack(undoStack, topundo);
                break;
            case 2:
                PrintStack(redoStack, topredo);
                break;
            default:
                break;
            }
        case 10:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 10);

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
