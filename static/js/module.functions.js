/**
 * 常用函数
 */
define([
        'jquery',
        'handlebars',
        'axios',
        'module.constants',
        'bootstrap'],
    function ($, Handlebars, axios, constants, _) {

        /**
         * 模板缓存
         * @type {{}}
         */
        let templateCache = {};

        /**
         * 弹出模态框
         * @param title
         * @param body
         */
        let modal = (title, body) => {
            $('.modal').modal('hide');
            $('#msgModalTitle').html(title);
            $('#msgModalBody').html(body);
            $('#msgModal').modal('show');
        };

        /**
         * 渲染模板
         * @param $el
         * @param renderStr
         * @param data
         */
        let render = ($el, renderStr, data) => {
            let template = Handlebars.compile(renderStr);
            let result = template(data);
            $el.html(result);
            return result;
        };

        /**
         * 加载模板
         * @param path
         * @returns {Promise<*>}
         */
        let loadTemplate = async (path) => {
            // 检查缓存
            if (path in templateCache) {
                return templateCache[path];
            }
            // 获取模板
            let url = constants.TEMPLATE_PATH + path + constants.TEMPLATE_SUFFIX;
            let response = await axios.get(url).catch(reason => {
                console.error("模板加载错误：", reason);
                return null;
            });
            // 写入缓存
            templateCache[path] = response.data;
            return response.data;
        };

        /**
         * 渲染HBS模板
         * @param $el
         * @param hbsPath
         * @param data
         */
        let renderHbs = async ($el, hbsPath, data) => {
            let renderStr = await loadTemplate(hbsPath);
            return render($el, renderStr, data);
        };

        /**
         * 页面跳转
         * @param dest
         * @param time
         */
        let jumpTo = (dest, time = 2000) => {
            if (time <= 0) {
                location.href = dest;
                return;
            }
            setTimeout(() => {
                location.href = dest;
            }, time);
        };

        /**
         * 刷新页面
         */
        let refresh = (time = 2000) => {
            if (time <= 0) {
                location.reload();
                return;
            }
            setTimeout(() => {
                location.reload();
            }, time);
        };

        /**
         * 时间戳转换成指定格式日期
         *
         * 时间格式：Y-m-d H:i:s
         *
         * @author zhangxinxu
         * @param timestamp
         * @param formats
         * @returns {string}
         */
        let dateFormatTs = function (timestamp, formats) {
            if (timestamp === null) {
                return "暂无";
            }

            formats = formats || 'Y-m-d';

            var zero = function (value) {
                if (value < 10) {
                    return '0' + value;
                }
                return value;
            };

            var myDate = timestamp? new Date(timestamp * 1000): new Date();

            var year = myDate.getFullYear();
            var month = zero(myDate.getMonth() + 1);
            var day = zero(myDate.getDate());

            var hour = zero(myDate.getHours());
            var minite = zero(myDate.getMinutes());
            var second = zero(myDate.getSeconds());

            return formats.replace(/Y|m|d|H|i|s/ig, function (matches) {
                return ({
                    Y: year,
                    m: month,
                    d: day,
                    H: hour,
                    i: minite,
                    s: second
                })[matches];
            });
        };

        /**
         * 日期转时间戳
         * @param date
         * @returns {number}
         */
        let dateToTs = (date) => {
            date = date.substring(0, 19);
            date = date.replace(/-/g,'/');
            return new Date(date).getTime() / 1000;
        };

        // 添加模态框
        let modalSrc = `
    <!-- 模态框 -->
<div class="modal fade" id="msgModal" tabindex="-1" role="dialog" aria-labelledby="msgModalTitle" aria-hidden="true">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="msgModalTitle">title</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body" id="msgModalBody">
                ...
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-secondary" data-dismiss="modal">关闭</button>
            </div>
        </div>
    </div>
</div>
    `;
        $(modalSrc).appendTo('body');

        return {
            requestParams: new URL(document.location.href).searchParams,
            
            modal: modal,
            render: render,
            renderHbs: renderHbs,
            jumpTo: jumpTo,
            refresh: refresh,
            loadTemplate: loadTemplate,
            dateFormatTs: dateFormatTs,
            dateToTs: dateToTs
        };
    });