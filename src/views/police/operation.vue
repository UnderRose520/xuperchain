<template>
  <div v-if="name == '公安局管理员'" class="dashboard-container">
    <div class="head">
      中华人民共和国公民身份证信息
      <!-- <el-button type="primary" plain class="add" @click="addInfo">添加</el-button> -->
      <el-button type="primary" plain class="add" @click="dialogFormVisible = true">添加</el-button>
    </div>
    <el-dialog title="添加信息" :visible.sync="dialogFormVisible" width="800px">
      <el-form :model="add">
        <el-form-item label="姓名" :label-width="searchLabelWidth"><el-input v-model="add.name" autocomplete="off" style="width: 500px" /></el-form-item>
        <el-form-item label="身份证" :label-width="searchLabelWidth"><el-input v-model="add.userid" autocomplete="off" style="width: 500px" /></el-form-item>
        <el-form-item label="性别" :label-width="searchLabelWidth"><el-input v-model="add.sex" autocomplete="off" style="width: 500px" /></el-form-item>
        <el-form-item label="民族" :label-width="searchLabelWidth"><el-input v-model="add.nation" autocomplete="off" style="width: 500px" /></el-form-item>
        <el-form-item label="家庭住址" :label-width="searchLabelWidth"><el-input v-model="add.address" autocomplete="off" style="width: 500px" /></el-form-item>
        <el-form-item label="有效期限" :label-width="searchLabelWidth"><el-input v-model="add.effectiveDate" autocomplete="off" style="width: 500px" /></el-form-item>
      </el-form>
      <div slot="footer" class="dialog-footer">
        <el-button @click="dialogFormVisible = false">取 消</el-button>
        <el-button type="primary" @click="addInfo">确 定</el-button>
      </div>
    </el-dialog>
    <el-table :data="tableData" style="width: 100%" max-height="1000" :row-class-name="tableRowClassName" border stripe>
      <el-table-column fixed prop="data.name" label="姓名" width="200" align="center" />
      <el-table-column prop="data.sex" label="性别" width="200" align="center" />
      <el-table-column prop="data.nation" label="民族" width="150" align="center" />
      <el-table-column prop="data.userid" label="身份证" width="350" align="center" />
      <el-table-column prop="data.address" label="家庭住址" width="400" align="center" />
      <el-table-column prop="data.effectiveDate" label="有效期限" width="150" align="center" />
      <el-table-column prop="hash.hash" label="存证哈希" width="350" align="center" />
    </el-table>
    <el-button type="primary" class="preview" @click="preview">预览证书</el-button>
    <el-dialog title="证书预览和下载" :visible.sync="dialogVisible" width="60%" :before-close="handleClose">
      <div id="pdfDom">
        <div class="proBox">
          <p class="tit">超级链公安局证书</p>

          <p class="con">
            姓名：
            <span class="con-name">{{ CA.name }}</span>
            <span class="code">编号：{{ CA.code }}</span>
          </p>
          <p class="con">
            性别：
            <span>{{ CA.sex }}</span>
          </p>
          <p class="con">民族：{{ CA.nation }}</p>
          <p class="con">身份证：{{ CA.userid }}</p>
          <p class="con">地址：{{ CA.address }}</p>
          <p class="con">有效期：{{ CA.effectiveDate }}</p>
          <!-- <p class="hash">Hash:{{ CA.hash }}</p> -->
          <div class="con-unit">
            <p class="jiguang">登记机关：</p>
            <p class="time">{{ CA.date }}</p>
          </div>
          <!--          <p class="proid">
              <span>编号：</span>
              <span>xxjj2021412</span>
            </p> -->
          <p class=" con-footer">云南大学山里孩子 监制</p>

          <div v-show="isShow" class="chapter"><canvas id="chapterCanvas" width="150" height="150" /></div>
        </div>
      </div>
      <span slot="footer" class="dialog-footer">
        <el-switch v-model="downType" inactive-color="#67c23a" active-text="图片下载" inactive-text="pdf下载" style="margin-right: 20px;" />
        <el-checkbox v-model="isShow" style="margin-right: 20px;">添加盖章</el-checkbox>
        <el-button @click="dialogVisible = false">取 消</el-button>
        <el-button type="primary" @click="getPdf('#pdfDom')">下载</el-button>
      </span>
    </el-dialog>
  </div>
  <div v-else class="dashboard-container">管理员权限不足！无法进行高级功能操作！</div>
</template>

<script>
import { mapGetters } from 'vuex'
import axios from 'axios'
import qs from 'qs'
export default {
  name: 'Operation',
  computed: {
    ...mapGetters(['name'])
  },
  data() {
    return {
      txid: '',
      dialogFormVisible: false,
      add: {
        userid: '',
        sex: '',
        nation: '',
        address: '',
        effectiveDate: '',
        name: ''
      },
      searchLabelWidth: '120px',

      tableData: [
        // {
        //   businessScope: "",
        //   operatingPeriod: "",
        //   address: "",
        //   charger: "",
        //   name: "",
        //   }
      ],
      data: '',
      hash: '',
      policeAddForm: {
        functionName: 'addPolice',
        sex: '',
        userid: '',
        nation: '',
        address: '',
        effectiveDate: '',
        name: ''
      },

      // 证书参数
      dialogVisible: false,
      pageData: null, // 接收html格式代码
      htmlTitle: '工商局证书',
      isShow: true,
      isCanvas: false,
      downType: true, // false为 pdf , true为图片
      CA: {
        name: '',
        sex: '',
        nation: '',
        userid: '',
        address: '',
        effectiveDate: '',
        hash: '',
        date: '',
        code: ''
      }
    }
  },
  mounted() {
    // response返回一个json{"data": "数据","status": "状态码","statusText":"状态文本","headers":{ "content-type": "application/json; charset=utf-8" },"config":"配置文件","method":"方法","url":"请求url","request":"请求体"}
    // axios
    //   .get('http://41t10474g3.qicp.vip/v1/line', { headers: { 'Content-Type': 'application/x-www-form-urlencoded' } })
    //   .then(res => {
    //     console.log(res.data.legend_data);
    //     for (let i = 0; i < res.data.legend_data.length; i++) {
    //       this.tableData.push(res.data.legend_data[i]);
    //     }
    //   })
    //   .catch(error => {
    //     console.log(error);
    //   });
  },
  methods: {
    deleteRow(index, rows) {
      rows.splice(index, 1)
    },
    tableRowClassName({ row, rowIndex }) {
      if (rowIndex === 1) {
        return 'warning-row'
      } else if (rowIndex === 3) {
        return 'success-row'
      }
      return ''
    },
    // addInfo() {
    //   const info = {
    //       Business_Scope: "服务",
    //       Operating_period: "2019年9月23号-2022年9月23号",
    //       address: "锦绣大街97号",
    //       charger: "王小虎",
    //       name: "王小虎"
    //     };
    //   this.tableData.push(info);
    // },
    handleClose() {
      this.dialogVisible = false
    },
    preview() {
      this.dialogVisible = true
      this.$nextTick(() => {
        if (!this.isCanvas) {
          // 只绘画一次
          this.isCanvas = true
          this.getChapter()
        }
      })
    },
    // 生成印章
    getChapter() {
      var canvas = document.getElementById('chapterCanvas')
      var context = canvas.getContext('2d')

      var text = 'xxx专用章'
      var companyName = '百度超级链工商局'
      //  context.translate(0, 0);
      // context.clearRect(0, 0, 200, 200);//清除整个画布
      // 绘制印章边框
      var width = canvas.width / 2
      var height = canvas.height / 2
      context.lineWidth = 3
      context.strokeStyle = '#cf0c0c'
      context.beginPath()
      context.arc(width, height, 60, 0, Math.PI * 2) // 宽、高、半径
      context.stroke()

      // 画五角星
      create5star(context, width, height, 15, '#cf0c0c', 0)

      // 绘制印章名称
      context.font = '20px 宋体'
      context.textBaseline = 'middle' // 设置文本的垂直对齐方式
      context.textAlign = 'center' // 设置文本的水平对对齐方式
      context.lineWidth = 1
      context.strokeStyle = '#cf0c0c'
      // context.strokeText(text, width, height + 30); // 名称 xx专用章

      // 绘制印章单位
      context.translate(width, height) // 平移到此位置,
      context.font = '20px 宋体'
      var count = companyName.length // 字数
      var angle = (5 * Math.PI) / (3 * (count - 1)) // 字间角度
      var chars = companyName.split('')
      var c
      for (var i = 0; i < count; i++) {
        c = chars[i] // 需要绘制的字符
        if (i == 0) {
          context.rotate((4 * Math.PI) / 6) // 文字起始位置
        } else {
          context.rotate(angle)
        }

        context.save()
        context.translate(45, 0) // 平移到此位置,此时字和x轴垂直，公司名称和最外圈的距离
        context.rotate(Math.PI / 2) // 旋转90度,让字平行于x轴
        context.strokeText(c, 0, 0) // 此点为字的中心点
        context.restore()
      }

      // 绘制五角星
      function create5star(context, sx, sy, radius, color, rotato) {
        context.save()
        context.fillStyle = color
        context.translate(sx, sy) // 移动坐标原点
        context.rotate(Math.PI + rotato) // 旋转
        context.beginPath() // 创建路径
        var x = Math.sin(0)
        var y = Math.cos(0)
        var dig = (Math.PI / 5) * 4
        for (var i = 0; i < 5; i++) {
          // 画五角星的五条边
          var x = Math.sin(i * dig)
          var y = Math.cos(i * dig)
          context.lineTo(x * radius, y * radius)
        }
        context.closePath()
        context.stroke()
        context.fill()
        context.restore()
      }
    },
    showDialog() {
      this.dialogFormVisible = true
    },
    	getProjectNum() {
      const projectTime = new Date() // 当前中国标准时间
      const Year = projectTime.getFullYear() // 获取当前年份 支持IE和火狐浏览器.
      const Month = projectTime.getMonth() + 1 // 获取中国区月份
      const Day = projectTime.getDate() // 获取几号
      var CurrentDate = Year
      if (Month >= 10) { // 判断月份和几号是否大于10或者小于10
        CurrentDate += Month
      } else {
        CurrentDate += '0' + Month
      }
      if (Day >= 10) {
        CurrentDate += Day
      } else {
        CurrentDate += '0' + Day
      }
      return CurrentDate
    },
    addInfo() {
      this.policeAddForm.userid = this.add.userid
      this.policeAddForm.name = this.add.name
      this.policeAddForm.sex = this.add.sex
      this.policeAddForm.address = this.add.address
      this.policeAddForm.nation = this.add.nation
      this.policeAddForm.effectiveDate = this.add.effectiveDate
      axios
        .post('http://41c484d417.zicp.vip/v1/policeAdd', qs.stringify(this.policeAddForm), { headers: { 'Content-Type': 'application/x-www-form-urlencoded' }})
        .then(res => {
          console.log(res.data)
          console.log(JSON.parse(res.data.legend_data[1]))
          this.data = JSON.parse(res.data.legend_data[1])
          this.hash = JSON.parse(res.data.hash)
          const DATATABLE = {
            data: this.data,
            hash: this.hash
          }

          this.tableData.push(DATATABLE)

          this.CA.name = this.data.name
          this.CA.userid = this.data.userid
          this.CA.address = this.data.address
          this.CA.nation = this.data.nation
          this.CA.effectiveDate = this.data.effectiveDate
          this.CA.sex = this.data.sex
          this.CA.hash = this.hash.hash

          const date = new Date()
          var res
          const y = date.getFullYear()
          let m = date.getMonth() + 1
          let d = date.getDate()
          let H = date.getHours()
          const mm = date.getMinutes()
          const s = date.getSeconds()
          m = m < 10 ? '0' + m : m
          d = d < 10 ? '0' + d : d
          H = H < 10 ? '0' + H : H
          // return y + "-" + m + "-" + d + " " + H + ":" + mm+":"+s;
          res = y + '-' + m + '-' + d + ' '
          this.CA.date = res

          // 调用获取当前日期的方法加四位随机数  赋值表单中的项目编号
          this.CA.code = this.getProjectNum() + Math.floor(Math.random() * 10000) // 如果是6位或者8位随机数，相应的 *1000000或者 *100000000就行了

          // console.log(DATATABLE);
        })
        .catch(error => {
          console.log(error)
        })
      // 关闭对话框
      this.dialogFormVisible = false
    }
  }
}
</script>

<style lang="scss" scoped>
.head {
  font-size: 28px;
  margin-left: 550px;
  font-weight: bold;
  margin-bottom: 10px;
  margin-top: 20px;
}
.add {
  margin-left: 590px;
  /* margin-top: -60px; */
}
.el-table .warning-row {
  background: oldlace;
}

.el-table .success-row {
  background: #f0f9eb;
}
.dashboard {
  &-container {
    margin: 10px;
  }
  &-text {
    font-size: 30px;
    line-height: 46px;
  }
}

::v-deep .el-dialog__body {
  padding: 0px;
  display: flex;
  justify-content: center;
}
#pdfDom {
  /* 要想pdf周边留白，要在这里设置 */
  padding: 20px;
  width: 750px;
}
.proBox {
  background: url('../../assets/CA.jpg') no-repeat;
  background-size: cover;
  width: 750px;
  height: 525px;
  padding: 60px 94px;
  box-sizing: border-box;
  margin: 0 auto;
  position: relative;
  color: #000;
  font-family: SimSun;
}
.tit {
  color: #cf0c0c;
  font-size: 32px;
  font-weight: 700;
  position: relative;
  top: -6px;
  left: 8px;
  text-align: center;
  letter-spacing: 20px;
  font-family: STHeiti;
  margin: 20px 0;
}
.proid {
  font-size: 15px;
  font-weight: 700;
  text-align: left;
  margin: 10px 0;
  line-height: 32px;
  text-indent: 2em;
}
.code {
  position: absolute;
  left: 430px;
}
.con {
  font-size: 20px;
  font-weight: 700;
  text-align: left;
  margin: 10px 0;
  line-height: 32px;
  text-indent: 2em;
}
.hash {
  /*  font-size: 18px;

  font-weight: bolder;
  text-align: left;
margin-left: -30px;
  line-height: 32px;
  text-indent: 2em; */
  text-align: left;
  /* margin: 0; */
  margin-left: 40px;
  font-weight: bold;
  font-size: 16px;
  /* white-space: normal; */
  /* font-weight:500; */
  /* font-family: 华文行楷, "microsoft yahei"; */
  font-family: '微软雅黑';
}
.con-name {
  font-family: 华文行楷, STXingkai;
  border-bottom: 2px solid #000;
}
.con-unit {
  font-size: 18px;
  font-weight: 700;
  position: absolute;
  right: 100px;
  bottom: 100px;
  text-align: center;
  letter-spacing: 3px;
}
.con-unit p {
  margin: 5px 50px;
}
.con-footer {
  font-size: 18px;
  font-weight: 700;
  position: absolute;
  bottom: 45px;
  left: 0;
  right: 0;
  text-align: center;
}
.chapter {
  border-radius: 50%;
  position: absolute;
  bottom: 75px;
  right: 134px;
}
.preview {
  margin-top: 30px;
  margin-left: 50px;
}
</style>
