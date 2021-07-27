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

// 城乡规划部存证上链存证API规范
// 参数由Context提供
//城乡规划部/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UrbanRural
{
public:
    // 初始化写入权限
    // 参数: owner - 具有写入权限的address
    virtual void UrbanRuralInitialize() = 0;

    // 写入规划许可证信息
    // 参数: userid;身份证，buildUnit：建设单位,projectName：项目名称,buildLocation：建设位置,buildScale：建设规模,issueDate：签发日期
    //      data - 规划许可证信息(json格式string)
    virtual void addUrbanRural() = 0;

    // 按照身份证查询规划许可证
    // 参数: userid：身份证
    // 返回值: data - 规划许可证信息(json格式string)
    virtual void queryUrbanRural() = 0;

    // 查询具有写权限的账户
    // 返回值: 具有写权限的address
    virtual void UrbanRuralQueryOwner() = 0;
};

class UrbanRuralData
{
public:
    std::string buildUnite;
    std::string projectname;
    std::string buildLocation;
    std::string buildScale;
    std::string issueDate;
};
class UrbanRuralVar
{
public:
    std::string buildUnite = "buildUnite";
    std::string projectname = "projectname";
    std::string buildLocation = "buildLocation";
    std::string buildScale = "buildScale";
    std::string issueDate = "issueDate";
    std::string userid = "userid";
};
struct UrbanRuralDemo : public UrbanRural, public xchain::Contract
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
    void UrbanRuralInitialize()
    {
        // 获取合约上下文对象
        xchain::Context *ctx = this->context();
        // 从合约上下文中获取合约参数, 由合约部署者指定具有写入权限的projectname
        std::string owner = ctx->arg("owner");
        if (owner.empty())
        {
            ctx->error("missing owner projectname");
            return;
        }
        // 将具有写入权限的owner地址记录在区块链账本中
        ctx->put_object(OWNER_KEY, owner);
        ctx->ok("success");
    }

    void addUrbanRural()
    {
        UrbanRuralData UrbanRuralData;
        UrbanRuralVar UrbanRuralVar;
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

        // 从参数中获取规划许可证主键，必填参数，没有则返回错误
        const std::string &userid = ctx->arg("userid");
        if (userid.empty())
        {
            ctx->error("missing 'userid'");
            return;
        }
        // 从参数中获取建设单位，必填参数，没有则返回错误
        UrbanRuralData.buildUnite = ctx->arg("buildUnite");
        if (UrbanRuralData.buildUnite.empty())
        {
            ctx->error("missing 'buildUnite'");
            return;
        }
        // 从参数中获取项目名称，必填参数，没有则返回错误
        UrbanRuralData.projectname = ctx->arg("projectname");
        if (UrbanRuralData.projectname.empty())
        {
            ctx->error("missing 'projectname'");
            return;
        }
        // 从参数中获取建设位置，必填参数，没有则返回错误
        UrbanRuralData.buildLocation = ctx->arg("buildLocation");
        if (UrbanRuralData.buildLocation.empty())
        {
            ctx->error("missing 'buildLocation'");
            return;
        }
        // 从参数中获取建设规模，必填参数，没有则返回错误
        UrbanRuralData.buildScale = ctx->arg("buildScale");
        if (UrbanRuralData.buildScale.empty())
        {
            ctx->error("missing 'buildScale'");
            return;
        }
        // 从参数中获取签发日期，必填参数，没有则返回错误
        UrbanRuralData.issueDate = ctx->arg("issueDate");
        if (UrbanRuralData.issueDate.empty())
        {
            ctx->error("missing 'issueDate'");
            return;
        }

        // 将具有写入权限的owner地址记录在区块链账本中
        std::string score_key = RECORD_KEY + userid;

        std::string res = "{\"" + UrbanRuralVar.buildUnite + "\":\"" + UrbanRuralData.buildUnite + "\"," + "\"" + UrbanRuralVar.projectname + "\":\"" + UrbanRuralData.projectname + "\"," + "\"" + UrbanRuralVar.buildLocation + "\":\"" + UrbanRuralData.buildLocation + "\"," + "\"" + UrbanRuralVar.buildScale + "\":\"" + UrbanRuralData.buildScale + "\"," + "\"" + UrbanRuralVar.issueDate + "\":\"" + UrbanRuralData.issueDate + "\","+"\"" + UrbanRuralVar.userid + "\":\"" + userid + "\"}";
        if (!ctx->put_object(score_key, res))
        {
            ctx->error("failed to save score record");
            return;
        }

        // 执行成功，返回status code 200
        ctx->ok(userid);
    }

    void queryUrbanRural()
    {
        // 获取合约上下文对象
        xchain::Context *ctx = this->context();
        UrbanRuralData UrbanRuralData1;
        UrbanRuralVar UrbanRuralVar1;
        // 从参数中获取身份证主键，必填参数，没有则返回错误
        const std::string &userid = ctx->arg("userid");
        if (userid.empty())
        {
            ctx->error("missing 'userid'");
            return;
        }

        // 从账本中读取规划许可证的数据
        std::string score_key = RECORD_KEY + userid;
        std::string data;
        if (!ctx->get_object(score_key, &data))
        {
            // 没查到，说明之前没上链过，返回错误
            ctx->error("no buildUnite record found of " + userid);
            return;
        }

        ctx->ok(data);
    }

    void UrbanRuralQueryOwner()
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
        // 执行成功，返回owner projectname
        ctx->ok(owner);
    }
};

//学生
DEFINE_METHOD(ScoreRecordDemo, initialize) { self.initialize(); }
DEFINE_METHOD(ScoreRecordDemo, addScore) { self.addScore(); }
DEFINE_METHOD(ScoreRecordDemo, queryScore) { self.queryScore(); }
DEFINE_METHOD(ScoreRecordDemo, queryOwner) { self.queryOwner(); }

//城乡规划部
DEFINE_METHOD(UrbanRuralDemo, UrbanRuralInitialize) { self.UrbanRuralInitialize(); }
DEFINE_METHOD(UrbanRuralDemo, addUrbanRural) { self.addUrbanRural(); }
DEFINE_METHOD(UrbanRuralDemo, queryUrbanRural) { self.queryUrbanRural(); }
DEFINE_METHOD(UrbanRuralDemo, UrbanRuralQueryOwner) { self.UrbanRuralQueryOwner(); }