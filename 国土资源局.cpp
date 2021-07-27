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

// 国土资源局存证上链存证API规范
// 参数由Context提供
//国土资源局///////////////////////////////////////////////////////////////////////////////////////////////////////
class Land
{
public:
    // 初始化写入权限
    // 参数: owner - 具有写入权限的address
    virtual void LandInitialize() = 0;

    // 写入土地使用证数据
    // 参数: userid：身份证,useName：使用者名称,address：地址,landNumber：地号,purpose：用途,serviceLife：使用期限
    //      data - 土地使用证信息(json格式string)
    virtual void addLand() = 0;

    // 按照身份证查询土地使用证信息
    // 参数: userid - 土地使用证的主键
    // 返回值: data - 土地使用证数据信息(json格式string)
    virtual void queryLand() = 0;

    // 查询具有写权限的账户
    // 返回值: 具有写权限的address
    virtual void LandQueryOwner() = 0;
};

class LandData
{
public:
    std::string useName;
    std::string address;
    std::string landNumber;
    std::string purpose;
    std::string serviceLife;
};
class LandVar
{
public:
    std::string useName = "useName";
    std::string address = "address";
    std::string landNumber = "landNumber";
    std::string purpose = "purpose";
    std::string serviceLife = "serviceLife";
    std::string userid = "userid";
};
struct LandDemo : public Land, public xchain::Contract
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
    void LandInitialize()
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

    void addLand()
    {
        LandData LandData;
        LandVar LandVar;
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

        // 从参数中获取土地使用证的主键，必填参数，没有则返回错误
        const std::string &userid = ctx->arg("userid");
        if (userid.empty())
        {
            ctx->error("missing 'userid'");
            return;
        }
        // 从参数中获取使用者名称，必填参数，没有则返回错误
        LandData.useName = ctx->arg("useName");
        if (LandData.useName.empty())
        {
            ctx->error("missing 'useName'");
            return;
        }
        // 从参数中获取地址，必填参数，没有则返回错误
        LandData.address = ctx->arg("address");
        if (LandData.address.empty())
        {
            ctx->error("missing 'address'");
            return;
        }
        // 从参数中获取地号，必填参数，没有则返回错误
        LandData.landNumber = ctx->arg("landNumber");
        if (LandData.landNumber.empty())
        {
            ctx->error("missing 'landNumber'");
            return;
        }
        // 从参数中获取用途，必填参数，没有则返回错误
        LandData.purpose = ctx->arg("purpose");
        if (LandData.purpose.empty())
        {
            ctx->error("missing 'purpose'");
            return;
        }
        // 从参数中获取使用期限，必填参数，没有则返回错误
        LandData.serviceLife = ctx->arg("serviceLife");
        if (LandData.serviceLife.empty())
        {
            ctx->error("missing 'serviceLife'");
            return;
        }

        // 将具有写入权限的owner地址记录在区块链账本中
        std::string score_key = RECORD_KEY + userid;

        std::string res = "{\"" + LandVar.useName + "\":\"" + LandData.useName + "\"," + "\"" + LandVar.address + "\":\"" + LandData.address + "\"," + "\"" + LandVar.landNumber + "\":\"" + LandData.landNumber + "\"," + "\"" + LandVar.purpose + "\":\"" + LandData.purpose + "\"," + "\"" + LandVar.serviceLife + "\":\"" + LandData.serviceLife +  "\","+"\"" + LandVar.userid + "\":\"" + userid + "\"}";
        if (!ctx->put_object(score_key, res))
        {
            ctx->error("failed to save score record");
            return;
        }

        // 执行成功，返回status code 200
        ctx->ok(userid);
    }

    void queryLand()
    {
        // 获取合约上下文对象
        xchain::Context *ctx = this->context();
        LandData LandData1;
        LandVar LandVar1;
        // 从参数中获取土地使用证主键，必填参数，没有则返回错误
        const std::string &userid = ctx->arg("userid");
        if (userid.empty())
        {
            ctx->error("missing 'userid'");
            return;
        }

        // 从账本中读取土地使用证的数据
        std::string score_key = RECORD_KEY + userid;
        std ::string data;
        if (!ctx->get_object(score_key, &data))
        {
            // 没查到，说明之前没上链过，返回错误
            ctx->error("no useName record found of " + userid);
            return;
        }

        ctx->ok(data);
    }

    void LandQueryOwner()
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

//国土资源局
DEFINE_METHOD(LandDemo, LandInitialize) { self.LandInitialize(); }
DEFINE_METHOD(LandDemo, addLand) { self.addLand(); }
DEFINE_METHOD(LandDemo, queryLand) { self.queryLand(); }
DEFINE_METHOD(LandDemo, LandQueryOwner) { self.LandQueryOwner(); }