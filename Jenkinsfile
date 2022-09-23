pipeline {
  agent any
  triggers {
  pollSCM('H/20 * * * *')
  }
  stages {
    stage('checkout scm') {
      steps {
        git 'eolJenkins:ncar/nimbus'
      }
    }
    stage('build') {
      steps {
        sh 'git submodule update --init --recursive'
        sh 'scons'
      }
    }
  }
  post {
    failure {
      emailext to: "cjw@ucar.edu janine@ucar.edu cdewerd@ucar.edu taylort@ucar.edu",
      subject: "Jenkinsfile nimbus build failed",
      body: "See console output attached",
      attachLog: true
    }
  }
  options {
    buildDiscarder(logRotator(numToKeepStr: '10'))
  }
}
