var fs = require('fs')
var gulp = require("gulp");
var gulpSync = require("gulp-sync")(gulp);
var path = require("path");
var exec = require('child_process').exec;
var sass = require("gulp-sass");
var concat = require('gulp-concat'),
    cssnano = require('gulp-cssnano'),
    autoprefixer = require('gulp-autoprefixer');


var _safeExec = (commandStr, handleErrFunc, handleSuccessFunc, done) => exec(commandStr, { maxBuffer: 1024 * 500 }, function (err, stdout, stderr) {
    if (err) {
        handleErrFunc(err, done);

        return;
    }

    handleSuccessFunc(done);
});

gulp.task("moveServerRouteType", function (done) {
    const serverRouteTypeDirPath = path.join(__dirname, "/../wonder-server/src/server/routes/type/routeType/");
    const targetRouteTypePath = path.join(__dirname, "src/core/routeType/");

    if (fs.existsSync(serverRouteTypeDirPath)) {
        fs.readdirSync(serverRouteTypeDirPath)
            .forEach((file) => {
                let filePath = serverRouteTypeDirPath + file;
                let fileJson = fs.readFileSync(filePath, "utf8");

                fs.writeFileSync(
                    targetRouteTypePath + file, fileJson, "utf8"
                );
            })
    }

    done();
});

gulp.task("moveGraphqlSchema", function (done) {
    const serverSchemaPath = path.join(__dirname, "/../wonder-server/graphql_schema.json");
    const targetSchemaPath = path.join(__dirname, "graphql_schema.json");

    var serverSchemaData = fs.readFileSync(serverSchemaPath, "utf8");
    console.log(serverSchemaData);

    fs.writeFileSync(
        targetSchemaPath, serverSchemaData, "utf8"
    );

    done();
});

gulp.task("sass", function () {
    return gulp.src('./public/sass/**/*.scss')
        .pipe(sass()).pipe(gulp.dest('./public/css'));
});

gulp.task("webpack:dev", function (done) {
    _safeExec("sudo npm run webpack:dev", (err, done) => { throw err }, (done) => done(), done);
});

gulp.task("webpack:prod", function (done) {
    _safeExec("sudo npm run webpack:prod", (err, done) => { throw err }, (done) => done(), done);
});


gulp.task("compressCss", function () {
    return gulp.src("./public/css/index.css")
        // TODO use index.min.css
        .pipe(concat("index.css"))
        .pipe(cssnano())
        .pipe(gulp.dest("./public/css"))
});

gulp.task("watchSass", function () {
    gulp.watch("public/sass/**/*.scss", ["sass"]);
});

gulp.task("watch", gulpSync.sync(["sass", "watchSass"]));

gulp.task("build", gulpSync.sync(["sass", "webpack:dev"]));

gulp.task("buildProd", gulpSync.sync(["sass", "webpack:prod", "compressCss"]));

gulp.task("getServerData", gulpSync.sync(["moveServerRouteType", "moveGraphqlSchema"]));