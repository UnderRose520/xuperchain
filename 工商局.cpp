#include "xchain/xchain.h"

// 学生成绩上链存证API规范
// 参数由Context提供
class ScoreRecord {
public:
    // 初始化写入权限
    // 参数: owner - 具有写入权限的address
    virtual void initialize() = 0;

    // 写入课程成绩
    // 参数: userid - 学生的主键id
    //      data - 学生的成绩信息(json格式string)
    virtual void addScore() = 0;

    // 按照学生id查询成绩
    // 参数: userid - 学生的主键id
    // 返回值: data - 学生的成绩信息(json格式string)
    virtual void queryScore() = 0;

    // 查询具有写权限的账户
    // 返回值: 具有写权限的address
    virtual void queryOwner() = 0;
};

struct ScoreRecordDemo : public ScoreRecord, public xchain::Contract {
private:
    // define the key prefix of buckets
    const std::string OWNER_KEY = "Owner";
    const std::string RECORD_KEY = "R_";

    // check if caller is the owner of this contract
    bool isOwner(xchain::Context* ctx, const std::string& caller) {
        std::string owner;
        if (!ctx->get_object(OWNER_KEY, &owner)) {
            return false;
        }
        return (owner == caller);
    }

public:
    void initialize() {
        // 获取合约上下文对象
        xchain::Context* ctx = this->context();
        // 从合约上下文中获取合约参数, 由合约部署者指定具有写入权限的address
        std::string owner = ctx->arg("owner");
        if (owner.empty()) {
            ctx->error("missing owner address");
            return;
        }
        // 将具有写入权限的owner地址记录在区块链账本中
        ctx->put_object(OWNER_KEY, owner);
        ctx->ok("success");
    }

    void addScore() {
        // 获取合约上下文对象
        xchain::Context* ctx = this->context();
        // 获取发起者身份
        const std::string& caller = ctx->initiator();
        if (caller.empty()) {
            ctx->error("missing initiator");
            return;
        }
        // 如果写操作发起者不是具有写权限的用户，则无权写入
        if (!isOwner(ctx, caller)) {
            ctx->error(
                "permission check failed, only the owner can add score record");
            return;
        }

        // 从参数中获取用户主键id，必填参数，没有则返回错误
        const std::string& userid = ctx->arg("userid");
        if (userid.empty()) {
            ctx->error("missing 'userid'");
            return;
        }
        // 从参数中获取成绩数据，必填参数，没有则返回错误
        const std::string& data = ctx->arg("data");
        if (data.empty()) {
            ctx->error("missing 'data'");
            return;
        }

        // 将具有写入权限的owner地址记录在区块链账本中
        std::string score_key = RECORD_KEY + userid;
        if (!ctx->put_object(score_key, data)) {
            ctx->error("failed to save score record");
            return;
        }

        // 执行成功，返回status code 200
        ctx->ok(userid);
    }

    void queryScore() {
        // 获取合约上下文对象
        xchain::Context* ctx = this->context();

        // 从参数中获取用户主键id，必填参数，没有则返回错误
        const std::string& userid = ctx->arg("userid");
        if (userid.empty()) {
            ctx->error("missing 'userid'");
            return;
        }

        // 从账本中读取学生的成绩数据
        std::string score_key = RECORD_KEY + userid;
        std::string data;
        if (!ctx->get_object(score_key, &data)) {
            // 没查到，说明之前没上链过，返回错误
            ctx->error("no score record found of " + userid);
            return;
        }

        // 执行成功，返回status code 200
        ctx->ok(data);
    }

    void queryOwner() {
        // 获取合约上下文对象
        xchain::Context* ctx = this->context();
        std::string owner;
        if (!ctx->get_object(OWNER_KEY, &owner)) {
            // 没查到owner信息，可能
            ctx->error("get owner failed");
            return;
        }
        // 执行成功，返回owner address
        ctx->ok(owner);
    }
};

#include "xchain/xchain.h"

// 工商局存证上链存证API规范
// 参数由Context提供
//工商局
class Business
{
public:
    // 初始化写入权限
    // 参数: owner - 具有写入权限的address
    virtual void businessInitialize() = 0;

    // 写入证书信息
    // 参数: userid - 学生的主键id
    //      data - 学生的成绩信息(json格式string)
    virtual void addBusiness() = 0;

    // 按照身份证查询营业执照
    // 参数: userid：身份证,name：名称,address：地址,charger：负责人,businessScope：经营范围,operatingPeriod：经营日期
    // 返回值: data - 营业执照的信息(json格式string)
    virtual void queryBusiness() = 0;

    // 查询具有写权限的账户
    // 返回值: 具有写权限的address
    virtual void businessQueryOwner() = 0;
};

class BusinessData
{
public:
    std::string name;
    std::string address;
    std::string charger;
    std::string businessScope;
    std::string operatingPeriod;
};
class BusinessVar
{
public:
    std::string name = "name";
    std::string address = "address";
    std::string charger = "charger";
    std::string businessScope = "businessScope";
    std::string operatingPeriod = "operatingPeriod";
    std::string userid = "userid";

};
struct BusinessDemo : public Business, public xchain::Contract
{
private:
    // define the key prefix of buckets
    const std::string OWNER_KEY = "Owner";
    const std::string RECORD_KEY = "R_";
    std::string userid;

    std::string score_key;
    // check if caller is the owner of this contract
    bool isOwner(xchain::Context *ctx, const std::string &caller)
    {
        std::string owner;
        if (!ctx->get_object(OWNER_KEY, &owner))
        {
            return false;
        }
        return (owner == caller);
    }

public:
    void businessInitialize()
    {
        // 获取合约上下文对象
        xchain::Context *ctx = this->context();
        // 从合约上下文中获取合约参数, 由合约部署者指定具有写入权限的address
        std::string owner = ctx->arg("owner");
        if (owner.empty())
        {
            ctx->error("missing owner address");
            return;
        }
        // 将具有写入权限的owner地址记录在区块链账本中
        ctx->put_object(OWNER_KEY, owner);
        ctx->ok("success");
    }

    void addBusiness()
    {
        BusinessData businessData;
        BusinessVar businessVar;
        // 获取合约上下文对象
        xchain::Context *ctx = this->context();
        // 获取发起者身份
        const std::string &caller = ctx->initiator();
        if (caller.empty())
        {
            ctx->error("missing initiator");
            return;
        }
        // 如果写操作发起者不是具有写权限的用户，则无权写入
        if (!isOwner(ctx, caller))
        {
            ctx->error(
                "permission check failed, only the owner can add score record");
            return;
        }

        // 从参数中获取营业执照的主键，必填参数，没有则返回错误
        const std::string &userid = ctx->arg("userid");
        if (userid.empty())
        {
            ctx->error("missing 'userid'");
            return;
        }
        // 从参数中获取姓名，必填参数，没有则返回错误
        businessData.name = ctx->arg("name");
        if (businessData.name.empty())
        {
            ctx->error("missing 'name'");
            return;
        }
        // 从参数中获取地址，必填参数，没有则返回错误
        businessData.address = ctx->arg("address");
        if (businessData.address.empty())
        {
            ctx->error("missing 'address'");
            return;
        }
        // 从参数中获取负责人数据，必填参数，没有则返回错误
        businessData.charger = ctx->arg("charger");
        if (businessData.charger.empty())
        {
            ctx->error("missing 'charger'");
            return;
        }
        // 从参数中获取经营范围，必填参数，没有则返回错误
        businessData.businessScope = ctx->arg("businessScope");
        if (businessData.businessScope.empty())
        {
            ctx->error("missing 'businessScope'");
            return;
        }
        // 从参数中获取经营时间，必填参数，没有则返回错误
        businessData.operatingPeriod = ctx->arg("operatingPeriod");
        if (businessData.operatingPeriod.empty())
        {
            ctx->error("missing 'operatingPeriod'");
            return;
        }

        // 将具有写入权限的owner地址记录在区块链账本中
        std::string score_key = RECORD_KEY + userid;

        std::string res = "{\"" + businessVar.name + "\":\"" + businessData.name + "\"," + "\"" + businessVar.address + "\":\"" + businessData.address + "\"," + "\"" +
                          businessVar.charger + "\":\"" + businessData.charger + "\"," + "\"" + businessVar.businessScope + "\":\"" + businessData.businessScope + "\"," + "\"" + businessVar.operatingPeriod + "\":\"" + businessData.operatingPeriod + "\","+"\"" + businessVar.userid + "\":\"" + userid + "\"}";
        if (!ctx->put_object(score_key, res))
        {
            ctx->error("failed to save score record");
            return;
        }

        // 执行成功，返回status code 200
        ctx->ok(userid);
    }

    void queryBusiness()
    {
        // 获取合约上下文对象
        xchain::Context *ctx = this->context();
        BusinessData businessData1;
        BusinessVar businessVar1;
        // 从参数中获取用户主键id，必填参数，没有则返回错误
        const std::string &userid = ctx->arg("userid");
        if (userid.empty())
        {
            ctx->error("missing 'userid'");
            return;
        }

        // 从账本中读取营业执照数据
        std::string score_key = RECORD_KEY + userid;
        std::string data;
        if (!ctx->get_object(score_key, &data))
        {
            // 没查到，说明之前没上链过，返回错误
            ctx->error("no name record found of " + userid);
            return;
        }

        ctx->ok(data);
    }

    void businessQueryOwner()
    {
        // 获取合约上下文对象
        xchain::Context *ctx = this->context();
        std::string owner;
        if (!ctx->get_object(OWNER_KEY, &owner))
        {
            // 没查到owner信息，可能
            ctx->error("get owner failed");
            return;
        }
        // 执行成功，返回owner address
        ctx->ok(owner);
    }
};

//学生
DEFINE_METHOD(ScoreRecordDemo, initialize) { self.initialize(); }
DEFINE_METHOD(ScoreRecordDemo, addScore) { self.addScore(); }
DEFINE_METHOD(ScoreRecordDemo, queryScore) { self.queryScore(); }
DEFINE_METHOD(ScoreRecordDemo, queryOwner) { self.queryOwner(); }

//工商局
DEFINE_METHOD(BusinessDemo, businessInitialize) { self.businessInitialize(); }
DEFINE_METHOD(BusinessDemo, addBusiness) { self.addBusiness(); }
DEFINE_METHOD(BusinessDemo, queryBusiness) { self.queryBusiness(); }
DEFINE_METHOD(BusinessDemo, businessQueryOwner) { self.businessQueryOwner(); }
