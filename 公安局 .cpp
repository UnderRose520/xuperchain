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

//公安局
class Police
{
public:
    // 初始化写入权限
    // 参数: owner - 具有写入权限的sex
    virtual void PoliceInitialize() = 0;

    // 写入身份证信息
    // 参数: userid - 主键身份证号
    //      data - 公民的身份证信息(json格式string)
    virtual void addPolice() = 0;

    // 按照身份证查询信息
    // 参数: userid - 主键身份证号
    // 返回值: data - 公民的身份证信息(json格式string)
    virtual void queryPolice() = 0;

    // 查询具有写权限的账户
    // 返回值: 具有写权限的sex
    virtual void PoliceQueryOwner() = 0;
};

class PoliceData
{
public:
    std::string name;//姓名
    std::string sex;//性别
    std::string nation;//民族
    std::string address;//地址
    std::string effectiveDate;//有效日期
};
class PoliceVar
{
public:
    std::string name = "name";
    std::string sex = "sex";
    std::string nation = "nation";
    std::string address = "address";
    std::string effectiveDate = "effectiveDate";
    std::string userid = "userid";
};
struct PoliceDemo : public Police, public xchain::Contract
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
    void PoliceInitialize()
    {
        // 获取合约上下文对象
        xchain::Context *ctx = this->context();
        // 从合约上下文中获取合约参数, 由合约部署者指定具有写入权限的sex
        std::string owner = ctx->arg("owner");
        if (owner.empty())
        {
            ctx->error("missing owner sex");
            return;
        }
        // 将具有写入权限的owner地址记录在区块链账本中
        ctx->put_object(OWNER_KEY, owner);
        ctx->ok("success");
    }

    void addPolice()
    {
        PoliceData PoliceData;
        PoliceVar PoliceVar;
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

        // 从参数中获取用户主键id（身份证），必填参数，没有则返回错误
        const std::string &userid = ctx->arg("userid");
        if (userid.empty())
        {
            ctx->error("missing 'userid'");
            return;
        }
        // 从参数中获取姓名，必填参数，没有则返回错误
        PoliceData.name = ctx->arg("name");
        if (PoliceData.name.empty())
        {
            ctx->error("missing 'name'");
            return;
        }
        // 从参数中获取性别，必填参数，没有则返回错误
        PoliceData.sex = ctx->arg("sex");
        if (PoliceData.sex.empty())
        {
            ctx->error("missing 'sex'");
            return;
        }
        // 从参数中获取民族，必填参数，没有则返回错误
        PoliceData.nation = ctx->arg("nation");
        if (PoliceData.nation.empty())
        {
            ctx->error("missing 'nation'");
            return;
        }
        // 从参数中获取地址，必填参数，没有则返回错误
        PoliceData.address = ctx->arg("address");
        if (PoliceData.address.empty())
        {
            ctx->error("missing 'address'");
            return;
        }
        // 从参数中获取有效时间，必填参数，没有则返回错误
        PoliceData.effectiveDate = ctx->arg("effectiveDate");
        if (PoliceData.effectiveDate.empty())
        {
            ctx->error("missing 'effectiveDate'");
            return;
        }

        // 将具有写入权限的owner地址记录在区块链账本中
        std::string score_key = RECORD_KEY + userid;

       //json格式
        std::string res = "{\"" + PoliceVar.name + "\":\"" + PoliceData.name + "\"," + "\"" + PoliceVar.sex + "\":\"" + PoliceData.sex + "\"," + "\"" + PoliceVar.nation + "\":\"" + PoliceData.nation + "\"," + "\"" + PoliceVar.address + "\":\"" + PoliceData.address + "\"," + "\"" + PoliceVar.effectiveDate + "\":\"" + PoliceData.effectiveDate + "\","+"\"" + PoliceVar.userid + "\":\"" + userid + "\"}";
        /*if (!ctx->put_object(score_key, name， sex， nation， address， effectiveDate)) {
            ctx->error("failed to save score record");*/
        if (!ctx->put_object(score_key, res))
        {
            ctx->error("failed to save score record");
            return;
        }

        // 执行成功，返回status code 200
        ctx->ok(userid);
    }

    void queryPolice()
    {
        // 获取合约上下文对象
        xchain::Context *ctx = this->context();
        PoliceData PoliceData1;
        PoliceVar PoliceVar1;
        // 从参数中获取用户主键id，必填参数，没有则返回错误
        const std::string &userid = ctx->arg("userid");
        if (userid.empty())
        {
            ctx->error("missing 'userid'");
            return;
        }

        // 从账本中读取身份证信息
        std::string score_key = RECORD_KEY + userid;
        std::string data;
        //if (!ctx->get_object(score_key, &name, &sex, &nation, &address, &effectiveDate))
        if (!ctx->get_object(score_key, &data))
        {
            // 没查到，说明之前没上链过，返回错误
            ctx->error("no name record found of " + userid);
            return;
        }

        ctx->ok(data);
    }

    void PoliceQueryOwner()
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
        // 执行成功，返回owner sex
        ctx->ok(owner);
    }
};


//学生

DEFINE_METHOD(ScoreRecordDemo, initialize) { self.initialize(); }
DEFINE_METHOD(ScoreRecordDemo, addScore) { self.addScore(); }
DEFINE_METHOD(ScoreRecordDemo, queryScore) { self.queryScore(); }
DEFINE_METHOD(ScoreRecordDemo,queryOwner) { self.queryOwner(); }

//公安局
DEFINE_METHOD(PoliceDemo, PoliceInitialize) { self.PoliceInitialize(); }
DEFINE_METHOD(PoliceDemo, addPolice) { self.addPolice(); }
DEFINE_METHOD(PoliceDemo, queryPolice) { self.queryPolice(); }
DEFINE_METHOD(PoliceDemo, PoliceQueryOwner) { self.PoliceQueryOwner(); }


