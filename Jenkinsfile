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
    success {
      mail(subject: 'nimbus Jenkinsfile build successful', body: 'nimbus Jenkinsfile build successful', to: 'cjw@ucar.edu janine@ucar.edu taylort@ucar.edu')
    }
  }
  options {
    buildDiscarder(logRotator(numToKeepStr: '10'))
  }
}
