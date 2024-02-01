FROM maven:3.9.6-eclipse-temurin-17-alpine AS build
WORKDIR /app
COPY pom.xml .
COPY src ./src
COPY front ./front
RUN mvn clean package -DskipTests

FROM eclipse-temurin:17.0.10_7-jre-alpine
WORKDIR /app
COPY --from=build /app/target/feeder-webapp.jar .
CMD ["java", "-jar", "feeder-webapp.jar"]