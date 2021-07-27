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


//房管局
class HousingAuthority
{
public:
    // 初始化写入权限
    // 参数: owner - 具有写入权限的preArea
    virtual void HousingAuthorityInitialize() = 0;

    // 写入课程成绩
    // 参数: userid - 主键id（身份证）
    //      data - 预售房许可证信息(json格式string)
    virtual void addHousingAuthority() = 0;

    // 按照主键id查询成绩
    // 参数: userid - 主键id（身份证）
    // 返回值: data - 预售房许可证信息(json格式string)
    virtual void queryHousingAuthority() = 0;

    // 查询具有写权限的账户
    // 返回值: 具有写权限的preArea
    virtual void HousingAuthorityQueryOwner() = 0;
};

class HousingAuthorityData
{
public:
    std::string preSeller;//预售人
    std::string preArea;//预售面积
    std::string projectName;//项目名称
    std::string usualSaleNum;//常房售号
    std::string issueDate;//签发日期
};
class HousingAuthorityVar
{
public:
    std::string preSeller = "preSeller";
    std::string preArea = "preArea";
    std::string projectName = "projectName";
    std::string usualSaleNum = "usualSaleNum";
    std::string issueDate = "issueDate";
    std::string userid = "userid";
};
struct HousingAuthorityDemo : public HousingAuthority, public xchain::Contract
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
    void HousingAuthorityInitialize()
    {
        // 获取合约上下文对象
        xchain::Context *ctx = this->context();
        // 从合约上下文中获取合约参数, 由合约部署者指定具有写入权限的preArea
        std::string owner = ctx->arg("owner");
        if (owner.empty())
        {
            ctx->error("missing owner preArea");
            return;
        }
        // 将具有写入权限的owner地址记录在区块链账本中
        ctx->put_object(OWNER_KEY, owner);
        ctx->ok("success");
    }

    void addHousingAuthority()
    {
        HousingAuthorityData HousingAuthorityData;
        HousingAuthorityVar HousingAuthorityVar;
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

        // 从参数中获取用户主键id，必填参数，没有则返回错误
        const std::string &userid = ctx->arg("userid");
        if (userid.empty())
        {
            ctx->error("missing 'userid'");
            return;
        }
        // 从参数中获取预售人，必填参数，没有则返回错误
        HousingAuthorityData.preSeller = ctx->arg("preSeller");
        if (HousingAuthorityData.preSeller.empty())
        {
            ctx->error("missing 'preSeller'");
            return;
        }
        // 从参数中获取预售面积，必填参数，没有则返回错误
        HousingAuthorityData.preArea = ctx->arg("preArea");
        if (HousingAuthorityData.preArea.empty())
        {
            ctx->error("missing 'preArea'");
            return;
        }
        // 从参数中获取项目名称，必填参数，没有则返回错误
        HousingAuthorityData.projectName = ctx->arg("projectName");
        if (HousingAuthorityData.projectName.empty())
        {
            ctx->error("missing 'projectName'");
            return;
        }
        // 从参数中获取常房售号，必填参数，没有则返回错误
        HousingAuthorityData.usualSaleNum = ctx->arg("usualSaleNum");
        if (HousingAuthorityData.usualSaleNum.empty())
        {
            ctx->error("missing 'usualSaleNum'");
            return;
        }
        // 从参数中获取签发日期，必填参数，没有则返回错误
        HousingAuthorityData.issueDate = ctx->arg("issueDate");
        if (HousingAuthorityData.issueDate.empty())
        {
            ctx->error("missing 'issueDate'");
            return;
        }

        // 将具有写入权限的owner地址记录在区块链账本中
        std::string score_key = RECORD_KEY + userid;

        std::string res = "{\"" + HousingAuthorityVar.preSeller + "\":\"" + HousingAuthorityData.preSeller + "\"," + "\"" + HousingAuthorityVar.preArea + "\":\"" + HousingAuthorityData.preArea + "\"," + "\"" + HousingAuthorityVar.projectName + "\":\"" + HousingAuthorityData.projectName + "\"," + "\"" + HousingAuthorityVar.usualSaleNum + "\":\"" + HousingAuthorityData.usualSaleNum + "\"," + "\"" + HousingAuthorityVar.issueDate + "\":\"" + HousingAuthorityData.issueDate + "\","+"\"" + HousingAuthorityVar.userid + "\":\"" + userid + "\"}";
        /*if (!ctx->put_object(score_key, preSeller， preArea， projectName， usualSaleNum， issueDate)) {
            ctx->error("failed to save score record");*/
        if (!ctx->put_object(score_key, res))
        {
            ctx->error("failed to save score record");
            return;
        }

        // 执行成功，返回status code 200
        ctx->ok(userid);
    }

    void queryHousingAuthority()
    {
        // 获取合约上下文对象
        xchain::Context *ctx = this->context();
        HousingAuthorityData HousingAuthorityData1;
        HousingAuthorityVar HousingAuthorityVar1;
        // 从参数中获取用户主键id，必填参数，没有则返回错误
        const std::string &userid = ctx->arg("userid");
        if (userid.empty())
        {
            ctx->error("missing 'userid'");
            return;
        }

        // 从账本中读取预售房许可证信息
        std::string score_key = RECORD_KEY + userid;
        std::string data;
        //if (!ctx->get_object(score_key, &preSeller, &preArea, &projectName, &usualSaleNum, &issueDate))
        if (!ctx->get_object(score_key, &data))
        {
            // 没查到，说明之前没上链过，返回错误
            ctx->error("no preSeller record found of " + userid);
            return;
        }
        ctx->ok(data);
    }

    void HousingAuthorityQueryOwner()
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
        // 执行成功，返回owner preArea
        ctx->ok(owner);
    }
};

//学生

DEFINE_METHOD(ScoreRecordDemo, initialize) { self.initialize(); }
DEFINE_METHOD(ScoreRecordDemo, addScore) { self.addScore(); }
DEFINE_METHOD(ScoreRecordDemo, queryScore) { self.queryScore(); }
DEFINE_METHOD(ScoreRecordDemo,queryOwner) { self.queryOwner(); }


//房管局
DEFINE_METHOD(HousingAuthorityDemo, HousingAuthorityInitialize) { self.HousingAuthorityInitialize(); }
DEFINE_METHOD(HousingAuthorityDemo, addHousingAuthority) { self.addHousingAuthority(); }
DEFINE_METHOD(HousingAuthorityDemo, queryHousingAuthority) { self.queryHousingAuthority(); }
DEFINE_METHOD(HousingAuthorityDemo, HousingAuthorityQueryOwner) { self.HousingAuthorityQueryOwner(); }
