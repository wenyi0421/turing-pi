<!DOCTYPE html>
<html lang="zh-CN" class="configure">

<head>

  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">

  <meta name="viewport" content="width=device-width, initial-scale=1">

  <meta http-equiv="Expires" content="0">
  <meta http-equiv="Pragma" content="no-cache">
  <meta http-equiv="Cache-control" content="no-cache">
  <meta http-equiv="Cache" content="no-cache">


  <title>turing config</title>


  <!--[if lt IE 9]>
      <script src="./js/html5shiv.min.js"></script>
      <script src="./js/respond.min.js"></script>
    <![endif]-->


  <link href="css/style.css" rel="stylesheet">
  <link href="css/css.css" rel="stylesheet">




  <!--new css-->
  <link rel="stylesheet" href="css/new/vendor/animate.css/animate.css">
  <link href="css/new/materialdesignicons.min.css" rel="stylesheet" type="text/css" />
  <link href="css/new/webfont.css" rel="stylesheet" type="text/css" />
  <link href="css/new/simple-line-icons.css" rel="stylesheet" type="text/css" />

  <link href="css/new/select2.min.css" rel="stylesheet" type="text/css" />
  <link href="css/new/bootstrap-tagsinput.css" rel="stylesheet" />
  <link href="css/new/switchery.min.css" rel="stylesheet">
  <link href="css/new/bootstrap-select.min.css" rel="stylesheet" />
  <link href="css/new/bootstrap-timepicker.min.css" rel="stylesheet" />
  <link href="css/new/bootstrap-colorpicker.min.css" rel="stylesheet" />
  <link href="css/new/bootstrap-datepicker.min.css" rel="stylesheet" />
  <link href="css/new/daterangepicker.css" rel="stylesheet" />

  <!-- Toastr css -->
  <link href="css/new/jquery.toast.min.css" rel="stylesheet" />

  <!-- ION Slider -->
  <link href="css/new/ion.rangeSlider.css" rel="stylesheet" type="text/css" />
  <link href="css/new/ion.rangeSlider.skinFlat.css" rel="stylesheet" type="text/css" />




  <!-- App css -->
  <!-- build:css -->
  <link href="css/new/app.css" rel="stylesheet" type="text/css" />

  <!--new css end-->

  <!--------------------------------- JQuery.JS ------------------------------------------------->
  <script src="js/jquery.min.js" type="text/javascript"></script>

  <!--------------------------------- slider.js ---------------------------------------------->
  <script src="js/new/ion.rangeSlider.min.js"></script>
  <script src="js/new/jquery.range-sliders.js"></script>

  <!--------------------------------- Fnuction ------------------------------------------------->
  <script src="js/new/m_page_disp.js" type="text/javascript"></script>


  <script type="text/javascript">

    //CB : http req sdcard set result
    function C_getCBResuleParsefactoryDisp(json) {
      console.log(json);

      $.each(json, function (index, item) {
        var sdcard   = json[index][0].sdcard;
        var n1pci    = json[index][0].n1pci;
        var n1ip     = json[index][0].n1ip;
        var n1ssd     = json[index][0].n1ssd;
        var n1usbotg = json[index][0].n1usbotg;

        var n2pci    = json[index][0].n2pci;
        var n2ip     = json[index][0].n2ip;
        var n2ssd     = json[index][0].n2ssd;
        var n2usbotg = json[index][0].n2usbotg;

        var n3pci    = json[index][0].n3pci;
        var n3ip     = json[index][0].n3ip;
        var n3ssd     = json[index][0].n3ssd;
        var n3usbotg = json[index][0].n3usbotg;
        var n3disk1  = json[index][0].n3disk1;
        var n3disk2  = json[index][0].n3disk2;

        var n4pci    = json[index][0].n4pci;
        var n4ip     = json[index][0].n4ip;
        var n4ssd     = json[index][0].n4ssd;
        var n4usbotg = json[index][0].n4usbotg;
        var n4usb1   = json[index][0].n4usb1;
        var n4usb2   = json[index][0].n4usb2;
        var n4usb3   = json[index][0].n4usb3;
        var n4usb4   = json[index][0].n4usb4;

        //
        $("#bmcsd").val(sdcard);
        $("#n1pci").val(n1pci);
        $("#n1ip").val(n1ip);
        $("#n1ssd").val(n1ssd);
        $("#n1usbotg").val(n1usbotg);
        $("#n2pci").val(n2pci);
        $("#n2ip").val(n2ip);
        $("#n2ssd").val(n2ssd);
        $("#n2usbotg").val(n2usbotg);
        $("#n3pci").val(n3pci);
        $("#n3ip").val(n3ip);
        $("#n3ssd").val(n3ssd);
        $("#n3usbotg").val(n3usbotg);
        $("#n3disk1").val(n3disk1);
        $("#n3disk2").val(n3disk2);
        $("#n4pci").val(n4pci);
        $("#n4ip").val(n4ip);
        $("#n4ssd").val(n4ssd);
        $("#n4usbotg").val(n4usbotg);
        $("#n4usb1").val(n4usb1);
        $("#n4usb2").val(n4usb2);
        $("#n4usb3").val(n4usb3);
        $("#n4usb4").val(n4usb4);
        // console.log('total:' + total);
      });
    }

    


    function C_getParamAll() {
      C_getParamList('factory');
    }


    function C_getParamList(ltype) {
      if (ltype == 'factory') {
        var url = '/api/bmc?opt=get&type=factory';
      }
      
      else
        return;

      page_http_req_get(url, ltype);
    }


  </script>
  <meta http-equiv="refresh" content="10"> <!-- 自动刷新 -->
</head>

<body onload="C_getParamAll()">
  <!-- <body> -->

  <!-- 中间部分 -->
  <div class="animated fadeInUp" style="position: absolute; left: 10px; top: 10px; width: 100%;">
    <div class="content_box clearfix">
      <div class="aui_nav_content_box">
        <div class="aui_nav_list_box">
          <ul class="clearfix">
          </ul>
        </div>


        <div class="aui_nav_list_tab">

          <div class="aui_nav_list_item">
            <ul>
              <div class="row">
                <div class="col-lg-6">

                  <form role="form" id="form1">
                    <h1>bmc</h1>
                    <hr style="height:1px;border:none;border-top:1px dashed #666;" />
                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">sdcard:</label>
                      <div class="col-sm-9">
                        <input type="text" id="bmcsd" class="form-control" disabled="true" />
                      </div>
                    </div>
                    <h1>node1</h1>
                    <hr style="height:1px;border:none;border-top:1px dashed #666;" />
                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">pcie插槽:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n1pci" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">ip地址:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n1ip" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">ssd:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n1ssd" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">usbotg:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n1usbotg" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <h1>node2</h1>
                    <hr style="height:1px;border:none;border-top:1px dashed #666;" />
                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">pcie插槽:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n2pci" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">ip地址:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n2ip" class="form-control" disabled="true" />
                      </div>
                    </div>
                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">ssd:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n2ssd" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">usbotg:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n2usbotg" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <h1>node3</h1>
                    <hr style="height:1px;border:none;border-top:1px dashed #666;" />
                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">pcie插槽:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n3pci" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">ip地址:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n3ip" class="form-control" disabled="true" />
                      </div>
                    </div>
                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">ssd:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n3ssd" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">usbotg:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n3usbotg" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">硬盘1:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n3disk1" class="form-control" disabled="true" />
                      </div>
                    </div>
                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">硬盘2:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n3disk2" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <h1>node4</h1>
                    <hr style="height:1px;border:none;border-top:1px dashed #666;" />
                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">pcie插槽:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n4pci" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">ip地址:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n4ip" class="form-control" disabled="true" />
                      </div>
                    </div>
                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">ssd:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n4ssd" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">usbotg:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n4usbotg" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">Usb1:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n4usb1" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">Usb2:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n4usb2" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">Usb3:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n4usb3" class="form-control" disabled="true" />
                      </div>
                    </div>

                    <div class="form-group row">
                      <label for="inputEmail3" class="col-sm-2 col-form-label">Usb4:</label>
                      <div class="col-sm-9">
                        <input type="text" id="n4usb4" class="form-control" disabled="true" />
                      </div>
                    </div>

                  </form>
                </div>
              </div>
            </ul>
          </div>
          </form>
        </div>
      </div>
      </ul>
    </div>

  </div>
  </div>
  </div>
  </div>


  <!-- 中间结束 -->

  <!--------------------------------- JS ------------------------------------------------->
  <!--  <script src="js/jquery.min.js"></script> -->
  <script src="js/bootstrap.min.js"></script>


  <!--new js-->
  <!-- jQuery  -->
  <script src="js/new/bootstrap.bundle.min.js"></script>
  <script src="js/new/jquery.slimscroll.min.js"></script>



  <!-- select2 js -->
  <script src="js/new/select2.min.js"></script>
  <script src="js/new/bootstrap-tagsinput.min.js"></script>
  <script src="js/new/switchery.min.js"></script>
  <script src="js/new/bootstrap-maxlength.min.js"></script>


  <!-- Mask input -->
  <script src="js/new/jquery.mask.min.js"></script>


  <!-- Bootstrap Select -->
  <script src="js/new/bootstrap-select.min.js"></script>
  <script src="js/new/bootstrap-timepicker.min.js"></script>
  <script src="js/new/bootstrap-colorpicker.min.js"></script>
  <script src="js/new/moment.js"></script>
  <script src="js/new/bootstrap-datepicker.min.js"></script>
  <script src="js/new/daterangepicker.js"></script>

  <!-- Toastr js -->
  <script src="js/new/jquery.toast.min.js"></script>
  <script src="js/new/jquery.toastr.js"></script>

  <!-- Init Js file -->
  <script src="js/new/jquery.form-advanced.js"></script>


  <!-- Parsley js -->
  <script type="text/javascript" src="js/new/parsley.min.js"></script>


  <script src="js/new/jquery.core.js"></script>
  <script src="js/new/jquery.app.js"></script>
  <!--new js end-->



  <script>
    $(function () {
      var num = sessionStorage.getItem('fromAVNum');
      $('.aui_nav_list_box').find('li').eq(num).addClass('aui_current').siblings().removeClass('aui_current');
      $('.aui_nav_list_tab>div:eq(' + num + ')').show().siblings().hide();
      $('.aui_nav_list_box li').click(function () {
        sessionStorage.setItem('fromAVNum', $(this).index());
        $(this).addClass('aui_current').siblings().removeClass('aui_current');
        $('.aui_nav_list_tab>div:eq(' + $(this).index() + ')').show().siblings().hide();
      })
    })
  </script>


  <script type="text/javascript">

    $('#form1').parsley();
    $('#form2').parsley();

    C_Notification();

  </script>

</body>

</html>
