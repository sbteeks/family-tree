#include <iostream>
#include <vector>

using namespace std;

/*
 * Code for building Family tree from the string that has pipe delimited nodes 
 * that represent family members. 
 *
 * Sample Input: "null,0,grandpa|0,1,son|0,2,daugther|1,3,grandkid"
 * 
 * This code assumes the each member can have max of two kids
 */

/*
 * Node class for Family Tree
 *
 */
class FamilyTreeNode {
    public:
        int parent_id;
        int node_id;
        string node_name;
        FamilyTreeNode *left;
        FamilyTreeNode *right;

    FamilyTreeNode(int p_id, int n_id, string n_name) {
        parent_id = p_id;
        node_id = n_id;
        node_name = n_name;
        left = NULL;
        right = NULL;
    }
};


/*
 * Creates FamilyTreeNode object
 *
 *  Params:
 *      member: string containing parent-id, node-id and node-name
 *
 *  Returns:
 *      Pointer to FamilyTreeNode object
 */
FamilyTreeNode* createFamilyMember(string member)
{

    string delim = ",";
    size_t start = 0U;
    size_t end = member.find(delim);

    vector<string> member_info;
    while(end != string::npos) {
        member_info.push_back(member.substr(start, end-start));
        start = end + delim.length();
        end = member.find(delim, start);
    }
    member_info.push_back(member.substr(start, end));

    int parent_id = (member_info[0] == "null") ? -1 : stoi(member_info[0]);
    int node_id = stoi(member_info[1]);
    string node_name = member_info[2];

    FamilyTreeNode* member_node = new FamilyTreeNode(parent_id, node_id, node_name);

    return member_node;
}

/*
 * Finds parent Node 
 *
 * Params: 
 *     root: top-level object of the tree
 *     parent_id: ID of the parent node to be searched
 *
 * Returns: Pointer to FamilyTreeNode object 
 */
FamilyTreeNode* findFamilyMemberParent(FamilyTreeNode* root, int parent_id) 
{
    if (root == NULL) {
        return NULL;
    }

    if (root->node_id == parent_id) {
        return root;
    }

    FamilyTreeNode* parent = findFamilyMemberParent(root->left, parent_id); 
    if (parent) {
        return parent;
    }

    return findFamilyMemberParent(root->right, parent_id); 
}

/* 
 *  Inserts a family member to the Family Tree
 *  
 *  Params:
 *     root: top-level object of the tree
 *     member: string containing parent-id, node-id and node-name

 *  Returns boolean:
 *     true: successfully inserted the object
 *     false: failed to insert 
 */
bool insertFamilyMember(FamilyTreeNode* root, string member)
{
    FamilyTreeNode* member_node = createFamilyMember(member);

    FamilyTreeNode* parent = findFamilyMemberParent(root, member_node->parent_id);

    if (parent) {
        if (parent->left == NULL) {
            parent->left = member_node;
        } else if (parent->right == NULL) {
            parent->right = member_node;
        } else {
            cout << "More that TWO children not allowed!!!\n";
            return false;
        } 
    }
    
    return true;
}

/*
 *  Creates Family Tree 
 *
 *  Params:
 *      members: string containing info about family members - contains 
 *               parent-id, node-id, node-name for each member
 */
FamilyTreeNode* createFamilyTree(string members)
{
    if (members.empty()) {
        return NULL;
    }  

    vector<string> members_list;
    string delim = "|";
    size_t start = 0U;
    size_t end = members.find(delim);
    
    while (end != string::npos) {
        members_list.push_back(members.substr(start, end-start));
        start = end + delim.length();
        end = members.find(delim, start);
    }
    members_list.push_back(members.substr(start, end));

    if (members_list.size() == 0) {
        return NULL;
    }

    FamilyTreeNode *root = createFamilyMember(members_list[0]);

    // Not a top level family member
    if (root->parent_id != -1) { 
        return NULL;
    }

    for (int idx = 1; idx < members_list.size(); idx++) {
        if (!insertFamilyMember(root, members_list[idx])) {
            return NULL;
        } 
    }

    return root;
}

void printFamilyTree(FamilyTreeNode* root)
{
    if (root == NULL) {
        return;
    }

    if (root->parent_id == -1) {
        cout << "Parent id: " << "null" << "\n";
    } else {
        cout << "Parent id: " << root->parent_id << "\n";
    }
    cout << "Node id: " <<  root->node_id << "\n";
    cout << "Node name: " << root->node_name << "\n\n";

    printFamilyTree(root->left);
    printFamilyTree(root->right);
}

/*
 * Driver code 
 */
int main()
{
    string members = "null,0,grandpa|0,1,son|0,2,daugther|1,3,grandkid|1,4,grandkid|2,5,grandkid|5,6,greatgrandkid";
    FamilyTreeNode *root = createFamilyTree(members);
    printFamilyTree(root);
    return 0;
}
