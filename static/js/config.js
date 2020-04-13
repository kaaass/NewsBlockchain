/**
 * RequireJS 配置
 */
;
requirejs.config({
    baseUrl: "/app/js",
    shim: {
        bootstrap: {
            deps: [ "jquery" ]
        }
    },
    paths: {
        axios: "https://cdn.bootcss.com/axios/0.19.0/axios.min",
        handlebars: "https://cdn.bootcss.com/handlebars.js/4.4.2/handlebars.min",
        bootstrap: "https://v4.bootcss.com/docs/4.3/dist/js/bootstrap.bundle",
        jquery: "https://cdn.bootcss.com/jquery/3.3.1/jquery.slim.min"
    },
    waitSeconds: 5
});
